#pragma once

#include <string>
#include <vector>
#include <optional>

#include "app/application.h"
#include "app/preview_storyboard.h"
#include "app/engine_shell_session.h"
#include "app/runtime_controller.h"
#include "app/shell_presenter.h"
#include "engine/preview_storyboard_renderer.h"
#include "simulators/lander/legacy/uiInteract.h"

class WindowShellRunner
{
public:
   WindowShellRunner();
   int run();

private:
   static void callback(const Interface * pUI, void * p);

   void onFrame(const Interface * pUI);
   void renderActiveFrame(ogstream & gout) const;
   void renderMenuScreen(ogstream & gout) const;
   void renderPlayMenu(ogstream & gout) const;
   void handleWindowMenuInput(const Interface * pUI);
   void handleWindowSceneInput(const Interface * pUI);
   void syncSelectionToState();
   std::vector<std::string> currentCommands() const;
   void submitCommand(const std::string & command);
   void drawOverlay(ogstream & gout) const;
   const PreviewStoryboard * currentPreviewStoryboard() const;

   Application app;
   RuntimeController controller;
   ShellPresenter presenter;
   EngineShellSession shellSession;
   PreviewStoryboardRenderer previewRenderer;
   Position viewport;
   AppState lastState;
   int selectionIndex;
   std::string lastNotice;
   mutable std::optional<SimulatorId> cachedPreviewId;
   mutable std::optional<PreviewStoryboard> cachedPreviewStoryboard;
};
