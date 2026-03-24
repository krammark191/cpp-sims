#include "engine/preview_storyboard_renderer.h"

#include <algorithm>

#include "simulators/common/scene_render_utils.h"

namespace
{
void drawImageGrid(const PreviewImage & image,
                   const Position & bottomLeft,
                   const Position & topRight,
                   ogstream & gout)
{
   if (image.width <= 0 || image.height <= 0 || image.pixels.empty())
      return;

   const double width = topRight.getX() - bottomLeft.getX();
   const double height = topRight.getY() - bottomLeft.getY();
   const double cellWidth = width / static_cast<double>(image.width);
   const double cellHeight = height / static_cast<double>(image.height);

   for (int row = 0; row < image.height; ++row)
   {
      for (int col = 0; col < image.width; ++col)
      {
         const auto & pixel = image.pixels[static_cast<std::size_t>(row * image.width + col)];
         const double left = bottomLeft.getX() + col * cellWidth;
         const double top = topRight.getY() - row * cellHeight;
         gout.drawRectangle(Position(left, top - cellHeight),
                            Position(left + cellWidth + 0.5, top + 0.5),
                            pixel.red,
                            pixel.green,
                            pixel.blue);
      }
   }
}
}

void PreviewStoryboardRenderer::render(const PreviewStoryboard & storyboard,
                                       double elapsedSeconds,
                                       const Position & bottomLeft,
                                       const Position & topRight,
                                       ogstream & gout) const
{
   if (storyboard.frames.empty())
      return;

   const std::size_t frameIndex = PreviewStoryboardLoader::frameIndexAtTime(storyboard, elapsedSeconds);
   const auto & frame = storyboard.frames[frameIndex];
   const double width = topRight.getX() - bottomLeft.getX();
   const double height = topRight.getY() - bottomLeft.getY();
   const double progressRatio =
      storyboard.frameSeconds > 0.0
         ? (elapsedSeconds - static_cast<double>(frameIndex) * storyboard.frameSeconds) / storyboard.frameSeconds
         : 0.0;
   const double clampedRatio = std::max(0.0, std::min(1.0, progressRatio));

   SceneRenderUtils::drawPanel(gout,
                               bottomLeft,
                               topRight,
                               frame.backgroundRed,
                               frame.backgroundGreen,
                               frame.backgroundBlue);
   SceneRenderUtils::drawOutline(gout,
                                 bottomLeft,
                                 topRight,
                                 frame.accentRed,
                                 frame.accentGreen,
                                 frame.accentBlue);

   const Position imageBottom(bottomLeft.getX() + 28.0, bottomLeft.getY() + 52.0);
   const Position imageTop(topRight.getX() - 28.0, topRight.getY() - 126.0);
   if (frame.image.has_value())
   {
      drawImageGrid(*frame.image, imageBottom, imageTop, gout);
      SceneRenderUtils::drawPanel(gout,
                                  Position(imageBottom.getX(), imageBottom.getY()),
                                  Position(imageTop.getX(), imageBottom.getY() + 28.0),
                                  frame.backgroundRed * 0.45,
                                  frame.backgroundGreen * 0.45,
                                  frame.backgroundBlue * 0.45);
   }

   const Position bandBottom(bottomLeft.getX() + 28.0, bottomLeft.getY() + height * 0.24);
   const Position bandTop(topRight.getX() - 28.0, bottomLeft.getY() + height * 0.36);
   SceneRenderUtils::drawPanel(gout,
                               bandBottom,
                               bandTop,
                               frame.accentRed * 0.45,
                               frame.accentGreen * 0.45,
                               frame.accentBlue * 0.45);

   const Position pulseCenter(bottomLeft.getX() + width * 0.72, bottomLeft.getY() + height * 0.60);
   SceneRenderUtils::drawMarker(gout,
                                pulseCenter,
                                18.0 + clampedRatio * 18.0,
                                frame.accentRed,
                                frame.accentGreen,
                                frame.accentBlue);
   SceneRenderUtils::drawCrosshair(gout, pulseCenter, 34.0 + clampedRatio * 12.0);

   gout.setPosition(Position(bottomLeft.getX() + 30.0, topRight.getY() - 38.0));
   gout << storyboard.title;
   gout.setPosition(Position(bottomLeft.getX() + 30.0, topRight.getY() - 76.0));
   gout << frame.headline;
   gout.setPosition(Position(bottomLeft.getX() + 30.0, topRight.getY() - 106.0));
   gout << frame.subline;

   const double progressWidth = width - 56.0;
   SceneRenderUtils::drawPanel(gout,
                               Position(bottomLeft.getX() + 28.0, bottomLeft.getY() + 26.0),
                               Position(topRight.getX() - 28.0, bottomLeft.getY() + 40.0),
                               0.08, 0.09, 0.11);
   SceneRenderUtils::drawPanel(gout,
                               Position(bottomLeft.getX() + 28.0, bottomLeft.getY() + 26.0),
                               Position(bottomLeft.getX() + 28.0 + progressWidth * clampedRatio,
                                        bottomLeft.getY() + 40.0),
                               frame.accentRed,
                               frame.accentGreen,
                               frame.accentBlue);
}
