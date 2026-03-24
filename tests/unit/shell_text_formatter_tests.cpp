#include <cassert>
#include <vector>

#include "app/shell_text_formatter.h"

void runShellTextFormatterTests()
{
   {
      const std::vector<std::string> commands{"play", "options", "quit"};
      assert(ShellTextFormatter::formatCommandFooter(commands) == "Commands: play options quit");
   }

   {
      assert(ShellTextFormatter::formatSelectedCommand("apollo") == "Selected: apollo");
   }
}
