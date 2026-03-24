#include "simulators/common/scene_render_utils.h"

#include <algorithm>
#include <sstream>

namespace SceneRenderUtils
{
namespace
{
double clamp01(double value)
{
   return std::max(0.0, std::min(1.0, value));
}

std::string joinLines(const std::string & title, const std::vector<std::string> & lines)
{
   std::ostringstream out;
   out << title;
   for (const std::string & line : lines)
      out << '\n' << line;
   return out.str();
}
}

void drawPanel(ogstream & gout,
               const Position & bottomLeft,
               const Position & topRight,
               double red,
               double green,
               double blue)
{
   gout.drawRectangle(bottomLeft, topRight, red, green, blue);
}

void drawMarker(ogstream & gout,
                const Position & center,
                double radius,
                double red,
                double green,
                double blue)
{
   gout.drawRectangle(Position(center.getX() - radius, center.getY() - radius),
                      Position(center.getX() + radius, center.getY() + radius),
                      red, green, blue);
}

void drawCrosshair(ogstream & gout,
                   const Position & center,
                   double radius,
                   double red,
                   double green,
                   double blue)
{
   gout.drawLine(Position(center.getX() - radius, center.getY()),
                 Position(center.getX() + radius, center.getY()),
                 red, green, blue);
   gout.drawLine(Position(center.getX(), center.getY() - radius),
                 Position(center.getX(), center.getY() + radius),
                 red, green, blue);
}

void drawOutline(ogstream & gout,
                 const Position & bottomLeft,
                 const Position & topRight,
                 double red,
                 double green,
                 double blue)
{
   gout.drawLine(bottomLeft, Position(topRight.getX(), bottomLeft.getY()), red, green, blue);
   gout.drawLine(bottomLeft, Position(bottomLeft.getX(), topRight.getY()), red, green, blue);
   gout.drawLine(topRight, Position(bottomLeft.getX(), topRight.getY()), red, green, blue);
   gout.drawLine(topRight, Position(topRight.getX(), bottomLeft.getY()), red, green, blue);
}

void drawTextPanel(ogstream & gout,
                   const Position & bottomLeft,
                   const Position & topRight,
                   const Position & textPosition,
                   const std::string & text,
                   double red,
                   double green,
                   double blue)
{
   drawPanel(gout, bottomLeft, topRight, red, green, blue);
   gout.setPosition(textPosition);
   gout << text;
}

void drawInfoPanel(ogstream & gout,
                   const Position & bottomLeft,
                   const Position & topRight,
                   const std::string & title,
                   const std::vector<std::string> & lines,
                   double red,
                   double green,
                   double blue)
{
   drawPanel(gout, bottomLeft, topRight, red, green, blue);
   drawOutline(gout,
               bottomLeft,
               topRight,
               clamp01(red + 0.10),
               clamp01(green + 0.10),
               clamp01(blue + 0.10));
   gout.setPosition(Position(bottomLeft.getX() + 14.0, topRight.getY() - 24.0));
   gout << joinLines(title, lines);
}

void drawLegendPanel(ogstream & gout,
                     const Position & bottomLeft,
                     const Position & topRight,
                     const std::string & title,
                     const std::vector<LegendEntry> & entries,
                     double red,
                     double green,
                     double blue)
{
   drawPanel(gout, bottomLeft, topRight, red, green, blue);
   drawOutline(gout,
               bottomLeft,
               topRight,
               clamp01(red + 0.10),
               clamp01(green + 0.10),
               clamp01(blue + 0.10));

   gout.setPosition(Position(bottomLeft.getX() + 14.0, topRight.getY() - 24.0));
   gout << title;

   double y = topRight.getY() - 50.0;
   for (const auto & entry : entries)
   {
      const Position marker(bottomLeft.getX() + 18.0, y + 4.0);
      drawMarker(gout, marker, 4.0, entry.red, entry.green, entry.blue);
      gout.setPosition(Position(bottomLeft.getX() + 34.0, y));
      gout << entry.label;
      y -= 22.0;
   }
}

void drawCenteredBanner(ogstream & gout,
                        const Position & viewport,
                        double yPosition,
                        const std::string & text)
{
   const double textWidth = static_cast<double>(text.length()) * 6.0;
   gout.setPosition(Position((viewport.getX() - textWidth) * 0.5, yPosition));
   gout << text;
}
}
