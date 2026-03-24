#include <cassert>

#include "app/application.h"
#include "app/input_state.h"
#include "app/isimulator_scene.h"

void runApplicationTests()
{
   {
      Application app;

      assert(app.state() == AppState::MainMenu);
      assert(!app.selectedSimulator().has_value());
      assert(app.controlsOverlayEnabled());
      assert(app.controlsOverlayDurationSeconds() == 5);
      assert(app.audioHooksEnabled());
   }

   {
      Application app;

      app.openPlayMenu();
      assert(app.state() == AppState::PlayMenu);
      assert(app.selectedPreviewMetadata() != nullptr);
      assert(app.playMenuSelectionIndex() == 0);
      assert(app.previewStoryboardAvailable());
      assert(app.previewAssetAvailable());

      const auto * initialPreview = app.selectedPreviewMetadata();
      assert(initialPreview != nullptr);
      app.tick(1.5);
      assert(app.previewPlaybackSeconds() > 1.0);
      assert(app.selectNextPreview());
      assert(app.playMenuSelectionIndex() == 1);
      assert(app.previewPlaybackSeconds() == 0.0);
      assert(app.selectedPreviewMetadata() != nullptr);
      assert(app.selectedPreviewMetadata()->id != initialPreview->id);
      assert(app.selectPreviousPreview());
      assert(app.playMenuSelectionIndex() == 0);

      app.openOptions();
      assert(app.state() == AppState::Options);

      app.openAbout();
      assert(app.state() == AppState::About);

      app.returnToMainMenu();
      assert(app.state() == AppState::MainMenu);
   }

   {
      Application app;

      assert(!app.openPauseMenu());
      assert(app.openSimulator(SimulatorId::Chess));
      assert(app.state() == AppState::InSimulator);
      assert(app.selectedPreviewMetadata() != nullptr);
      assert(app.selectedPreviewMetadata()->id == SimulatorId::Chess);
      assert(app.hasActiveScene());
      assert(app.activeScene() != nullptr);
      assert(app.activeScene()->id() == SimulatorId::Chess);
      assert(!app.activeScene()->isPlaceholder());
      assert(app.activeScene()->isReady());
      assert(app.activeScene()->supportsFutureAiModes());
      assert(app.controlsOverlayEnabled());
      assert(app.openPauseMenu());
      assert(app.state() == AppState::PauseMenu);
      assert(app.openKeybindings());
      assert(app.state() == AppState::Keybindings);
      assert(app.resumeSimulator());
      assert(app.state() == AppState::InSimulator);
      assert(app.quitSimulatorToMenu());
      assert(app.state() == AppState::MainMenu);
      assert(!app.selectedSimulator().has_value());
   }

   {
      Application app;

      assert(app.openSimulator(SimulatorId::ApolloLander));
      app.requestQuit();
      assert(app.quitRequested());
      assert(app.state() == AppState::ExitRequested);
      assert(!app.hasActiveScene());
      assert(!app.selectedSimulator().has_value());
   }

   {
      Application app;

      assert(app.openSimulator(SimulatorId::ApolloLander));
      assert(app.hasActiveScene());
      assert(app.activeScene() != nullptr);
      assert(app.activeScene()->id() == SimulatorId::ApolloLander);
      assert(!app.activeScene()->isPlaceholder());
      assert(app.activeScene()->isReady());
      assert(app.activeSceneKeybindings().size() == 4);
      assert(app.controlsOverlayEnabled());

      app.tick(3.0);
      assert(app.controlsOverlayEnabled());

      app.tick(2.1);
      assert(!app.controlsOverlayEnabled());
      assert(app.activeScene()->elapsedSeconds() > 5.0);

      assert(app.quitSimulatorToMenu());
      assert(!app.hasActiveScene());
   }

   {
      Application app;
      InputState input;

      assert(app.openSimulator(SimulatorId::ApolloLander));
      const auto * beforeScene = app.activeScene();
      assert(beforeScene != nullptr);

      input.mainThrust = true;
      app.setInputState(input);
      app.tick(1.0);
      app.clearInputState();

      const auto * afterScene = app.activeScene();
      assert(afterScene != nullptr);
      assert(afterScene == beforeScene);
      assert(afterScene->elapsedSeconds() > 0.0);
   }

   {
      Application app;
      InputState input;

      assert(app.openSimulator(SimulatorId::Howitzer));
      assert(app.activeScene() != nullptr);
      assert(!app.activeScene()->isPlaceholder());

      input.firePressed = true;
      app.setInputState(input);
      app.tick(0.1);
      app.clearInputState();

      assert(app.activeSceneElapsedSeconds() > 0.0);
   }

   {
      Application app;
      InputState input;

      assert(app.openSimulator(SimulatorId::Chess));
      assert(app.activeScene() != nullptr);
      assert(!app.activeScene()->isPlaceholder());

      input.firePressed = true;
      input.selectPressed = true;
      app.setInputState(input);
      app.tick(0.1);
      app.clearInputState();

      assert(app.activeSceneElapsedSeconds() > 0.0);
      assert(app.activeSceneStatusText() != nullptr);
   }

   {
      Application app;
      InputState input;

      assert(app.openSimulator(SimulatorId::Orbital));
      assert(app.activeScene() != nullptr);
      assert(!app.activeScene()->isPlaceholder());

      input.mainThrust = true;
      input.firePressed = true;
      app.setInputState(input);
      app.tick(0.1);
      app.clearInputState();

      assert(app.activeSceneElapsedSeconds() > 0.0);
      assert(app.activeSceneStatusText() != nullptr);
   }
}
