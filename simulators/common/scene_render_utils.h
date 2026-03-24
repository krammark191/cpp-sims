#pragma once

#include <vector>
#include <string>

#include "simulators/lander/legacy/uiDraw.h"

namespace SceneRenderUtils
{
struct LegendEntry
{
   std::string label;
   double red = 1.0;
   double green = 1.0;
   double blue = 1.0;
};

void drawPanel(ogstream & gout,
               const Position & bottomLeft,
               const Position & topRight,
               double red,
               double green,
               double blue);

void drawMarker(ogstream & gout,
                const Position & center,
                double radius,
                double red,
                double green,
                double blue);

void drawCrosshair(ogstream & gout,
                   const Position & center,
                   double radius,
                   double red = 1.0,
                   double green = 0.95,
                   double blue = 0.35);

void drawOutline(ogstream & gout,
                 const Position & bottomLeft,
                 const Position & topRight,
                 double red,
                 double green,
                 double blue);

void drawTextPanel(ogstream & gout,
                   const Position & bottomLeft,
                   const Position & topRight,
                   const Position & textPosition,
                   const std::string & text,
                   double red,
                   double green,
                   double blue);

void drawInfoPanel(ogstream & gout,
                   const Position & bottomLeft,
                   const Position & topRight,
                   const std::string & title,
                   const std::vector<std::string> & lines,
                   double red,
                   double green,
                   double blue);

void drawLegendPanel(ogstream & gout,
                     const Position & bottomLeft,
                     const Position & topRight,
                     const std::string & title,
                     const std::vector<LegendEntry> & entries,
                     double red,
                     double green,
                     double blue);

void drawCenteredBanner(ogstream & gout,
                        const Position & viewport,
                        double yPosition,
                        const std::string & text);
}
