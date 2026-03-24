#include <cassert>

#include "app/runtime_controller.h"

void runRuntimeControllerTests()
{
   RuntimeController controller;

   {
      Application app;
      const auto result = controller.handleCommand(app, "list");
      assert(result.continueRunning);
   }

   {
      Application app;
      auto result = controller.handleCommand(app, "play");
      assert(result.continueRunning);
      assert(app.state() == AppState::PlayMenu);
      const auto firstPreview = app.selectedPreviewMetadata();
      assert(firstPreview != nullptr);

      result = controller.handleCommand(app, "next");
      assert(result.continueRunning);
      assert(app.selectedPreviewMetadata() != nullptr);
      assert(app.selectedPreviewMetadata()->id != firstPreview->id);

      result = controller.handleCommand(app, "prev");
      assert(result.continueRunning);
      assert(app.selectedPreviewMetadata() != nullptr);
      assert(app.selectedPreviewMetadata()->id == firstPreview->id);

      result = controller.handleCommand(app, "open");
      assert(result.continueRunning);
      assert(app.state() == AppState::InSimulator);
      assert(!result.notices.empty());
      assert(app.openPauseMenu());
      result = controller.handleCommand(app, "quit");
      assert(result.continueRunning);
      assert(app.state() == AppState::MainMenu);
   }

   {
      Application app;
      auto result = controller.handleCommand(app, "play");
      assert(result.continueRunning);
      assert(app.state() == AppState::PlayMenu);

      result = controller.handleCommand(app, "apollo");
      assert(result.continueRunning);
      assert(app.state() == AppState::InSimulator);
      assert(!result.notices.empty());

      result = controller.handleCommand(app, "pause");
      assert(result.continueRunning);
      assert(app.state() == AppState::PauseMenu);

      result = controller.handleCommand(app, "keybindings");
      assert(result.continueRunning);
      assert(app.state() == AppState::Keybindings);

      result = controller.handleCommand(app, "resume");
      assert(result.continueRunning);
      assert(app.state() == AppState::InSimulator);

      result = controller.handleCommand(app, "back");
      assert(result.continueRunning);
      assert(app.state() == AppState::MainMenu);
   }

   {
      Application app;
      auto result = controller.handleCommand(app, "howitzer");
      assert(result.continueRunning);
      result = controller.handleCommand(app, "step 0.5 wf");
      assert(result.continueRunning);
      assert(app.activeSceneElapsedSeconds() > 0.0);
   }

   {
      Application app;
      const auto result = controller.handleCommand(app, "quit");
      assert(!result.continueRunning);
   }
}
