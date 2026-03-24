#include "app/shell_text_formatter.h"

namespace ShellTextFormatter
{
std::string formatCommandFooter(const std::vector<std::string> & commands)
{
   std::string footer = "Commands:";
   for (const auto & command : commands)
      footer += " " + command;
   return footer;
}

std::string formatSelectedCommand(const std::string & command)
{
   return "Selected: " + command;
}
}
