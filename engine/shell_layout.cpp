#include "engine/shell_layout_builder.h"

#include <algorithm>

namespace
{
double centeredX(double viewportWidth, const std::string & text, double charWidth)
{
   const double textWidth = static_cast<double>(text.size()) * charWidth;
   return std::max(16.0, (viewportWidth - textWidth) / 2.0);
}
}

ShellLayout ShellLayoutBuilder::build(const ShellScreen & screen,
                                      double viewportWidth,
                                      double viewportHeight) const
{
   ShellLayout layout;

   if (!screen.title.empty())
   {
      layout.lines.push_back(
      {
         centeredX(viewportWidth, screen.title, 8.0),
         viewportHeight * 0.90,
         screen.title
      });
   }

   if (!screen.subtitle.empty())
   {
      layout.lines.push_back(
      {
         centeredX(viewportWidth, screen.subtitle, 7.0),
         viewportHeight * 0.84,
         screen.subtitle
      });
   }

   double bodyY = viewportHeight * 0.74;
   for (const auto & line : screen.bodyLines)
   {
      layout.lines.push_back({viewportWidth * 0.08, bodyY, line});
      bodyY -= 24.0;
   }

   if (!screen.footerCommands.empty())
   {
      std::string footer = "Commands:";
      for (const auto & command : screen.footerCommands)
      {
         footer += " " + command;
      }

      layout.lines.push_back(
      {
         viewportWidth * 0.05,
         viewportHeight * 0.08,
         footer
      });
   }

   return layout;
}
