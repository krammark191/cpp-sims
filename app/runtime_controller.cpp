#include "app/runtime_controller.h"

#include <string>
#include <sstream>

#include "app/input_state.h"
#include "app/scene_input_router.h"

namespace
{
bool parseStepCommand(const std::string & command,
                      double & seconds,
                      std::string & keys)
{
   std::istringstream stream(command);
   std::string verb;
   if (!(stream >> verb) || verb != "step")
      return false;

   if (!(stream >> seconds))
      return false;

   stream >> keys;
   return true;
}

bool openNamedScene(Application & app, const std::string & command)
{
   if (command == "apollo")
      return app.openSimulator(SimulatorId::ApolloLander);
   if (command == "howitzer")
      return app.openSimulator(SimulatorId::Howitzer);
   if (command == "chess")
      return app.openSimulator(SimulatorId::Chess);
   if (command == "orbital")
      return app.openSimulator(SimulatorId::Orbital);
   return false;
}

RuntimeCommandResult handleMenuState(Application & app, const std::string & command)
{
   RuntimeCommandResult result;

   if (command == "quit")
   {
      result.continueRunning = false;
      result.renderScreen = false;
      return result;
   }

   if (command == "list" || command == "menu")
      return result;

   if (app.state() == AppState::PlayMenu)
   {
      if (command == "next")
      {
         app.selectNextPreview();
         return result;
      }

      if (command == "prev")
      {
         app.selectPreviousPreview();
         return result;
      }

      if (command == "open")
      {
         if (app.openSelectedPreview())
            result.notices.push_back("Opened " + std::string(app.activeSceneName()));
         return result;
      }
   }

   if (command == "play")
   {
      app.openPlayMenu();
      return result;
   }

   if (command == "options")
   {
      app.openOptions();
      return result;
   }

   if (command == "about")
   {
      app.openAbout();
      return result;
   }

   if (command == "back")
   {
      app.returnToMainMenu();
      return result;
   }

   if (openNamedScene(app, command))
   {
      result.notices.push_back("Opened " + std::string(app.activeSceneName()));
      return result;
   }

   result.notices.push_back("Unknown menu command");
   return result;
}

RuntimeCommandResult handleSceneState(Application & app, const std::string & command)
{
   RuntimeCommandResult result;

   if (command == "back")
   {
      app.quitSimulatorToMenu();
      return result;
   }

   if (command == "pause")
   {
      app.openPauseMenu();
      return result;
   }

    if (command == "status")
      return result;

   if (command == "bindings" || command == "keybindings")
   {
      app.openKeybindings();
      return result;
   }

   if (command == "reset")
   {
      const auto id = app.selectedSimulator();
      if (!id.has_value())
         return result;
      app.quitSimulatorToMenu();
      app.openSimulator(*id);
      result.notices.push_back("Scene reset");
      return result;
   }

   double seconds = 0.0;
   std::string keys;
   if (parseStepCommand(command, seconds, keys))
   {
      const auto id = app.selectedSimulator();
      if (!id.has_value())
         return result;

      app.setInputState(SceneInputRouter::fromStepKeys(*id, keys));
      app.tick(seconds);
      app.clearInputState();
      return result;
   }

   result.notices.push_back("Unknown scene command");
   return result;
}

RuntimeCommandResult handlePauseState(Application & app, const std::string & command)
{
   RuntimeCommandResult result;

   if (command == "resume" || command == "back")
   {
      app.resumeSimulator();
      result.notices.push_back("Resumed " + std::string(app.activeSceneName()));
      return result;
   }

   if (command == "keybindings" || command == "bindings")
   {
      app.openKeybindings();
      return result;
   }

   if (command == "quit")
   {
      app.quitSimulatorToMenu();
      return result;
   }

   result.notices.push_back("Unknown pause command");
   return result;
}

RuntimeCommandResult handleKeybindingState(Application & app, const std::string & command)
{
   RuntimeCommandResult result;

   if (command == "resume" || command == "back")
   {
      app.resumeSimulator();
      result.notices.push_back("Resumed " + std::string(app.activeSceneName()));
      return result;
   }

   if (command == "quit")
   {
      app.quitSimulatorToMenu();
      return result;
   }

   return result;
}
}

RuntimeCommandResult RuntimeController::handleCommand(Application & app,
                                                      const std::string & command) const
{
   switch (app.state())
   {
   case AppState::MainMenu:
   case AppState::PlayMenu:
   case AppState::Options:
   case AppState::About:
      return handleMenuState(app, command);

   case AppState::InSimulator:
      return handleSceneState(app, command);

   case AppState::PauseMenu:
      return handlePauseState(app, command);

   case AppState::Keybindings:
      return handleKeybindingState(app, command);

   case AppState::ExitRequested:
      return RuntimeCommandResult{false, false, {}};
   }

   return RuntimeCommandResult{false, false, {}};
}
