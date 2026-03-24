#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "app/app_state.h"
#include "app/control_binding.h"
#include "app/input_state.h"
#include "app/isimulator_scene.h"
#include "app/simulator_id.h"
#include "app/simulator_metadata.h"

class Application
{
public:
   Application();

   AppState state() const;
   std::optional<SimulatorId> selectedSimulator() const;
   bool controlsOverlayEnabled() const;
   int controlsOverlayDurationSeconds() const;
   bool audioHooksEnabled() const;
   bool quitRequested() const;
   bool hasActiveScene() const;
   const ISimulatorScene * activeScene() const;
   const char * activeSceneName() const;
   const char * activeSceneStatusText() const;
   const std::vector<ControlBinding> & activeSceneKeybindings() const;
   double activeSceneElapsedSeconds() const;
   const SimulatorMetadata * selectedPreviewMetadata() const;
   std::size_t playMenuSelectionIndex() const;
   double previewPlaybackSeconds() const;
   double previewLoopDurationSeconds() const;
   bool previewAssetAvailable() const;
   bool previewStoryboardAvailable() const;

   void openPlayMenu();
   void openOptions();
   void openAbout();
   bool openSimulator(SimulatorId id);
   bool openPauseMenu();
   bool resumeSimulator();
   bool openKeybindings();
   bool quitSimulatorToMenu();
   bool selectPreview(SimulatorId id);
   bool selectNextPreview();
   bool selectPreviousPreview();
   bool openSelectedPreview();
   void returnToMainMenu();
   void requestQuit();
   void setInputState(const InputState & input);
   void clearInputState();
   void tick(double dtSeconds);

private:
   AppState currentState;
   std::optional<SimulatorId> currentSimulator;
   bool showControlsOverlay;
   int controlsOverlaySeconds;
   bool audioSettingsHooksReady;
   InputState currentInput;
    std::size_t previewSelectionIndex;
   double previewSeconds;
   double previewLoopSeconds;
   std::unique_ptr<ISimulatorScene> scene;
};
