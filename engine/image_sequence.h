#pragma once

#include <string>
#include <vector>

struct ImageSequencePixel
{
   double red = 0.0;
   double green = 0.0;
   double blue = 0.0;
   double alpha = 0.0;
};

struct ImageSequenceFrame
{
   int width = 0;
   int height = 0;
   std::vector<ImageSequencePixel> pixels;

   bool empty() const
   {
      return width <= 0 || height <= 0 || pixels.empty();
   }
};

namespace ImageSequenceLoader
{
std::vector<ImageSequenceFrame> loadPngSequence(const std::string & directoryPath);
std::vector<ImageSequenceFrame> loadPngSpriteStrip(const std::string & filePath,
                                                   int frameWidth,
                                                   int frameHeight);
}
