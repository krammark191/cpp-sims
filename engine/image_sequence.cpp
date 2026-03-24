#include "engine/image_sequence.h"

#include <algorithm>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#if defined(__APPLE__) && defined(CPP_SIMULATORS_WINDOW)
#include <ApplicationServices/ApplicationServices.h>
#elif defined(_WIN32) && defined(CPP_SIMULATORS_WINDOW)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <wincodec.h>
#endif

namespace
{
#if defined(__APPLE__) && defined(CPP_SIMULATORS_WINDOW)
std::optional<ImageSequenceFrame> loadSinglePng(const std::filesystem::path & file)
{
   CFURLRef url = CFURLCreateFromFileSystemRepresentation(
      kCFAllocatorDefault,
      reinterpret_cast<const UInt8 *>(file.string().c_str()),
      static_cast<CFIndex>(file.string().size()),
      false);
   if (url == nullptr)
      return std::nullopt;

   CGImageSourceRef source = CGImageSourceCreateWithURL(url, nullptr);
   CFRelease(url);
   if (source == nullptr)
      return std::nullopt;

   CGImageRef image = CGImageSourceCreateImageAtIndex(source, 0, nullptr);
   CFRelease(source);
   if (image == nullptr)
      return std::nullopt;

   const std::size_t width = CGImageGetWidth(image);
   const std::size_t height = CGImageGetHeight(image);
   if (width == 0 || height == 0)
   {
      CGImageRelease(image);
      return std::nullopt;
   }

   std::vector<unsigned char> raw(width * height * 4, 0);
   CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
   CGContextRef context = CGBitmapContextCreate(raw.data(),
                                                width,
                                                height,
                                                8,
                                                width * 4,
                                                colorSpace,
                                                kCGImageAlphaPremultipliedLast |
                                                   kCGBitmapByteOrder32Big);
   CGColorSpaceRelease(colorSpace);
   if (context == nullptr)
   {
      CGImageRelease(image);
      return std::nullopt;
   }

   CGContextDrawImage(context, CGRectMake(0.0, 0.0, width, height), image);
   CGContextRelease(context);
   CGImageRelease(image);

   ImageSequenceFrame frame;
   frame.width = static_cast<int>(width);
   frame.height = static_cast<int>(height);
   frame.pixels.reserve(width * height);
   for (std::size_t i = 0; i < width * height; ++i)
   {
      const std::size_t offset = i * 4;
      frame.pixels.push_back(
      {
         static_cast<double>(raw[offset + 0]) / 255.0,
         static_cast<double>(raw[offset + 1]) / 255.0,
         static_cast<double>(raw[offset + 2]) / 255.0,
         static_cast<double>(raw[offset + 3]) / 255.0
      });
   }

   return frame;
}
#elif defined(_WIN32) && defined(CPP_SIMULATORS_WINDOW)
std::optional<ImageSequenceFrame> loadSinglePng(const std::filesystem::path & file)
{
   HRESULT initResult = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
   const bool shouldUninitialize = SUCCEEDED(initResult);

   IWICImagingFactory * factory = nullptr;
   IWICBitmapDecoder * decoder = nullptr;
   IWICBitmapFrameDecode * sourceFrame = nullptr;
   IWICFormatConverter * converter = nullptr;

   auto cleanup = [&]()
   {
      if (converter != nullptr)
         converter->Release();
      if (sourceFrame != nullptr)
         sourceFrame->Release();
      if (decoder != nullptr)
         decoder->Release();
      if (factory != nullptr)
         factory->Release();
      if (shouldUninitialize)
         CoUninitialize();
   };

   if (FAILED(CoCreateInstance(CLSID_WICImagingFactory,
                               nullptr,
                               CLSCTX_INPROC_SERVER,
                               IID_IWICImagingFactory,
                               reinterpret_cast<void **>(&factory))))
   {
      cleanup();
      return std::nullopt;
   }

   const std::wstring widePath = file.wstring();
   if (FAILED(factory->CreateDecoderFromFilename(widePath.c_str(),
                                                 nullptr,
                                                 GENERIC_READ,
                                                 WICDecodeMetadataCacheOnLoad,
                                                 &decoder)))
   {
      cleanup();
      return std::nullopt;
   }

   if (FAILED(decoder->GetFrame(0, &sourceFrame)))
   {
      cleanup();
      return std::nullopt;
   }

   if (FAILED(factory->CreateFormatConverter(&converter)))
   {
      cleanup();
      return std::nullopt;
   }

   if (FAILED(converter->Initialize(sourceFrame,
                                    GUID_WICPixelFormat32bppRGBA,
                                    WICBitmapDitherTypeNone,
                                    nullptr,
                                    0.0,
                                    WICBitmapPaletteTypeCustom)))
   {
      cleanup();
      return std::nullopt;
   }

   UINT width = 0;
   UINT height = 0;
   if (FAILED(converter->GetSize(&width, &height)) || width == 0 || height == 0)
   {
      cleanup();
      return std::nullopt;
   }

   std::vector<unsigned char> raw(static_cast<std::size_t>(width) * height * 4, 0);
   const UINT stride = width * 4;
   const UINT rawSize = static_cast<UINT>(raw.size());
   if (FAILED(converter->CopyPixels(nullptr, stride, rawSize, raw.data())))
   {
      cleanup();
      return std::nullopt;
   }

   ImageSequenceFrame frame;
   frame.width = static_cast<int>(width);
   frame.height = static_cast<int>(height);
   frame.pixels.reserve(static_cast<std::size_t>(width) * height);
   for (std::size_t i = 0; i < static_cast<std::size_t>(width) * height; ++i)
   {
      const std::size_t offset = i * 4;
      frame.pixels.push_back(
      {
         static_cast<double>(raw[offset + 0]) / 255.0,
         static_cast<double>(raw[offset + 1]) / 255.0,
         static_cast<double>(raw[offset + 2]) / 255.0,
         static_cast<double>(raw[offset + 3]) / 255.0
      });
   }

   cleanup();
   return frame;
}
#endif
}

std::vector<ImageSequenceFrame> ImageSequenceLoader::loadPngSequence(const std::string & directoryPath)
{
   std::vector<ImageSequenceFrame> frames;

#if defined(__APPLE__) && defined(CPP_SIMULATORS_WINDOW)
   const std::filesystem::path directory(directoryPath);
   if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory))
      return frames;

   std::vector<std::filesystem::path> files;
   for (const auto & entry : std::filesystem::directory_iterator(directory))
   {
      if (!entry.is_regular_file())
         continue;

      if (entry.path().extension() == ".png")
         files.push_back(entry.path());
   }

   std::sort(files.begin(), files.end());

   for (const auto & file : files)
   {
      if (auto frame = loadSinglePng(file))
         frames.push_back(std::move(*frame));
   }
#else
   (void)directoryPath;
#endif

   return frames;
}

std::vector<ImageSequenceFrame> ImageSequenceLoader::loadPngSpriteStrip(const std::string & filePath,
                                                                        int frameWidth,
                                                                        int frameHeight)
{
   std::vector<ImageSequenceFrame> frames;

#if defined(__APPLE__) && defined(CPP_SIMULATORS_WINDOW)
   if (frameWidth <= 0 || frameHeight <= 0)
      return frames;

   auto strip = loadSinglePng(std::filesystem::path(filePath));
   if (!strip.has_value() || strip->empty())
      return frames;

   if (strip->width < frameWidth || strip->height < frameHeight)
      return frames;

   const int columns = strip->width / frameWidth;
   const int rows = strip->height / frameHeight;
   for (int row = 0; row < rows; ++row)
   {
      for (int col = 0; col < columns; ++col)
      {
         ImageSequenceFrame frame;
         frame.width = frameWidth;
         frame.height = frameHeight;
         frame.pixels.reserve(static_cast<std::size_t>(frameWidth * frameHeight));
         for (int y = 0; y < frameHeight; ++y)
         {
            for (int x = 0; x < frameWidth; ++x)
            {
               const int sourceX = col * frameWidth + x;
               const int sourceY = row * frameHeight + y;
               frame.pixels.push_back(
                  strip->pixels[static_cast<std::size_t>(sourceY * strip->width + sourceX)]);
            }
         }
         frames.push_back(std::move(frame));
      }
   }
#else
   (void)filePath;
   (void)frameWidth;
   (void)frameHeight;
#endif

   return frames;
}
