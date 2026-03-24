#include <cassert>

#include "app/shell_command_catalog.h"

void runShellCommandCatalogTests()
{
   {
      const auto commands = ShellCommandCatalog::forState(AppState::MainMenu);
      assert(commands.size() == 4);
      assert(commands.front() == "play");
      assert(commands.back() == "quit");
   }

   {
      const auto commands = ShellCommandCatalog::forState(AppState::InSimulator);
      assert(commands.size() == 6);
      assert(commands[0] == "status");
      assert(commands[5] == "back");
   }

   {
      const auto commands = ShellCommandCatalog::forState(AppState::ExitRequested);
      assert(commands.empty());
   }
}
