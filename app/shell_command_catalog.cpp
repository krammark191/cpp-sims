#include "app/shell_command_catalog.h"

namespace ShellCommandCatalog
{
std::vector<std::string> forState(AppState state)
{
   switch (state)
   {
   case AppState::MainMenu:
      return {"play", "options", "about", "quit"};
   case AppState::PlayMenu:
      return {"prev", "next", "open", "apollo", "howitzer", "chess", "orbital", "back"};
   case AppState::Options:
   case AppState::About:
      return {"back"};
   case AppState::PauseMenu:
      return {"resume", "keybindings", "quit"};
   case AppState::Keybindings:
      return {"resume", "quit"};
   case AppState::InSimulator:
      return {"status", "step <seconds> <keys>", "pause", "keybindings", "reset", "back"};
   case AppState::ExitRequested:
      return {};
   }

   return {};
}
}
