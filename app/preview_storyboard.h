#pragma once

#include <optional>
#include <string>
#include <vector>

#include "app/simulator_metadata.h"

struct PreviewPixel
{
   double red = 0.0;
   double green = 0.0;
   double blue = 0.0;
};

struct PreviewImage
{
   int width = 0;
   int height = 0;
   std::vector<PreviewPixel> pixels;
};

struct PreviewStoryboardFrame
{
   std::string headline;
   std::string subline;
   std::string imagePath;
   double backgroundRed = 0.08;
   double backgroundGreen = 0.10;
   double backgroundBlue = 0.14;
   double accentRed = 0.92;
   double accentGreen = 0.80;
   double accentBlue = 0.24;
   std::optional<PreviewImage> image;
};

struct PreviewStoryboard
{
   std::string title;
   double frameSeconds = 2.0;
   std::vector<PreviewStoryboardFrame> frames;
};

namespace PreviewStoryboardLoader
{
std::optional<PreviewStoryboard> load(const SimulatorMetadata & metadata);
bool hasStoryboard(const SimulatorMetadata & metadata);
std::size_t frameIndexAtTime(const PreviewStoryboard & storyboard, double seconds);
}
