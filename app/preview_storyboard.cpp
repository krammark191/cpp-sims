#include "app/preview_storyboard.h"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <limits>
#include <sstream>

#include "app/runtime_paths.h"

namespace
{
std::string trim(const std::string & value)
{
   const auto begin = value.find_first_not_of(" \t\r\n");
   if (begin == std::string::npos)
      return "";

   const auto end = value.find_last_not_of(" \t\r\n");
   return value.substr(begin, end - begin + 1);
}

bool parseColor(const std::string & value,
                double & red,
                double & green,
                double & blue)
{
   std::istringstream stream(value);
   char comma1 = '\0';
   char comma2 = '\0';
   if (!(stream >> red >> comma1 >> green >> comma2 >> blue))
      return false;

   return comma1 == ',' && comma2 == ',';
}

std::optional<PreviewImage> loadPortablePixmap(const std::filesystem::path & path)
{
   std::ifstream input(path);
   if (!input.is_open())
      return std::nullopt;

   std::string magic;
   input >> magic;
   if (magic != "P3")
      return std::nullopt;

   auto skipComments = [&input]()
   {
      while (input >> std::ws && input.peek() == '#')
         input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
   };

   skipComments();
   PreviewImage image;
   int maxValue = 255;
   if (!(input >> image.width >> image.height))
      return std::nullopt;

   skipComments();
   if (!(input >> maxValue) || image.width <= 0 || image.height <= 0 || maxValue <= 0)
      return std::nullopt;

   image.pixels.reserve(static_cast<std::size_t>(image.width * image.height));
   for (int i = 0; i < image.width * image.height; ++i)
   {
      skipComments();
      int red = 0;
      int green = 0;
      int blue = 0;
      if (!(input >> red >> green >> blue))
         return std::nullopt;

      image.pixels.push_back(
      {
         static_cast<double>(red) / maxValue,
         static_cast<double>(green) / maxValue,
         static_cast<double>(blue) / maxValue
      });
   }

   return image;
}
}

std::optional<PreviewStoryboard> PreviewStoryboardLoader::load(const SimulatorMetadata & metadata)
{
   if (metadata.previewManifestPath == nullptr)
      return std::nullopt;

   const std::filesystem::path manifestPath =
      RuntimePaths::resolveRelativePath(std::filesystem::path(metadata.previewManifestPath));
   std::ifstream input(manifestPath);
   if (!input.is_open())
      return std::nullopt;

   PreviewStoryboard storyboard;
   PreviewStoryboardFrame currentFrame;
   bool inFrame = false;
   bool hasCurrentFrame = false;
   std::string line;
   while (std::getline(input, line))
   {
      line = trim(line);
      if (line.empty() || line[0] == '#')
         continue;

      if (line == "[frame]")
      {
         if (hasCurrentFrame)
            storyboard.frames.push_back(currentFrame);

         currentFrame = PreviewStoryboardFrame{};
         inFrame = true;
         hasCurrentFrame = true;
         continue;
      }

      const auto separator = line.find('=');
      if (separator == std::string::npos)
         continue;

      const std::string key = trim(line.substr(0, separator));
      const std::string value = trim(line.substr(separator + 1));

      if (!inFrame)
      {
         if (key == "title")
            storyboard.title = value;
         else if (key == "frame_seconds")
            storyboard.frameSeconds = std::stod(value);
         continue;
      }

      if (key == "headline")
         currentFrame.headline = value;
      else if (key == "subline")
         currentFrame.subline = value;
      else if (key == "image")
      {
         currentFrame.imagePath = value;
         currentFrame.image = loadPortablePixmap(manifestPath.parent_path() / value);
      }
      else if (key == "background")
         parseColor(value,
                    currentFrame.backgroundRed,
                    currentFrame.backgroundGreen,
                    currentFrame.backgroundBlue);
      else if (key == "accent")
         parseColor(value,
                    currentFrame.accentRed,
                    currentFrame.accentGreen,
                    currentFrame.accentBlue);
   }

   if (hasCurrentFrame)
      storyboard.frames.push_back(currentFrame);

   if (storyboard.frames.empty())
      return std::nullopt;

   if (storyboard.title.empty())
      storyboard.title = metadata.displayName;
   if (storyboard.frameSeconds <= 0.0)
      storyboard.frameSeconds = 2.0;

   return storyboard;
}

bool PreviewStoryboardLoader::hasStoryboard(const SimulatorMetadata & metadata)
{
   return metadata.previewManifestPath != nullptr &&
      std::filesystem::exists(
         RuntimePaths::resolveRelativePath(std::filesystem::path(metadata.previewManifestPath)));
}

std::size_t PreviewStoryboardLoader::frameIndexAtTime(const PreviewStoryboard & storyboard,
                                                      double seconds)
{
   if (storyboard.frames.empty())
      return 0;

   const double frameSeconds = storyboard.frameSeconds > 0.0 ? storyboard.frameSeconds : 2.0;
   const auto frame = static_cast<std::size_t>(std::floor(seconds / frameSeconds));
   return frame % storyboard.frames.size();
}
