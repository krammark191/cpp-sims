#include "app/shell_presenter.h"

#include <ostream>

#include "app/control_binding_formatter.h"
#include "app/scene_status_formatter.h"
#include "app/shell_command_catalog.h"
#include "app/shell_text_formatter.h"
#include "app/simulator_registry.h"

namespace
{
void appendBindings(const Application & app, ShellScreen & screen)
{
   const auto lines = ControlBindingFormatter::formatAll(app.activeSceneKeybindings(), true);
   screen.bodyLines.insert(screen.bodyLines.end(), lines.begin(), lines.end());
}

void appendSimulatorSummaries(ShellScreen & screen)
{
   for (const auto & simulator : SimulatorRegistry::all())
      screen.bodyLines.push_back(std::string("- ") + simulator.displayName + ": " + simulator.summary);
}
}

ShellScreen ShellPresenter::build(const Application & app) const
{
   ShellScreen screen;

   switch (app.state())
   {
   case AppState::MainMenu:
      screen.title = "C++ Simulators";
      screen.subtitle = "Main Menu";
      screen.bodyLines =
      {
         "Sampler shell for Apollo 11 Lander, Howitzer, Chess, and Orbital.",
         "Play opens the prerecorded preview browser before launching a simulator.",
         "Options tracks current hooks and release targets for the first macOS build.",
         "About summarizes the merged project and simulator roster."
      };
      screen.footerCommands = ShellCommandCatalog::forState(app.state());
      break;

   case AppState::PlayMenu:
      {
      const auto * preview = app.selectedPreviewMetadata();
      screen.title = "Play";
      screen.subtitle = preview ? preview->displayName : "Select a simulator";
      if (preview)
      {
         screen.bodyLines.push_back(std::string("Summary: ") + preview->summary);
         screen.bodyLines.push_back(
            std::string("Preview source: ") +
            (app.previewStoryboardAvailable() ? preview->previewManifestPath : preview->demoVideoPath) +
            (app.previewAssetAvailable()
               ? (app.previewStoryboardAvailable() ? " (storyboard ready)" : " (video ready)")
               : " (missing)"));
         screen.bodyLines.push_back(
            "Loop time: " +
            std::to_string(static_cast<int>(app.previewPlaybackSeconds())) + " / " +
            std::to_string(static_cast<int>(app.previewLoopDurationSeconds())) + " s");
         screen.bodyLines.push_back("Simulators:");
      }

      for (std::size_t i = 0; i < SimulatorRegistry::all().size(); ++i)
      {
         const auto & simulator = SimulatorRegistry::all()[i];
         const bool selected = i == app.playMenuSelectionIndex();
         screen.bodyLines.push_back(std::string(selected ? "> " : "  ") + simulator.displayName);
      }

      screen.footerCommands = ShellCommandCatalog::forState(app.state());
      break;
      }

   case AppState::Options:
      screen.title = "Options";
      screen.subtitle = "Current hooks and release defaults";
      screen.bodyLines =
      {
         "Audio: hooks ready, no music or effects assets imported yet.",
         "Controls overlay duration: " + std::to_string(app.controlsOverlayDurationSeconds()) + " s on scene entry.",
         "Pause behavior: ESC opens Resume / Keybindings / Quit while the scene is frozen.",
         "Preview playback: storyboard loop backend active for the Play screen.",
         "Packaging target: macOS dmg release plus a Windows executable release package.",
         "Future toggles: master volume, music volume, SFX volume, fullscreen, overlay duration."
      };
      screen.footerCommands = ShellCommandCatalog::forState(app.state());
      break;

   case AppState::About:
      screen.title = "About";
      screen.subtitle = "Merged project sampler";
      screen.bodyLines =
      {
         "This sampler merges four sequential BYU-Idaho C++ simulator projects under one shell.",
         "Simulator descriptions:"
      };
      appendSimulatorSummaries(screen);
      screen.bodyLines.push_back("Authorship:");
      screen.bodyLines.push_back("- Mark Van Horn and Taden Marston authored the preexisting files carrying their names.");
      screen.bodyLines.push_back("- Dr. James Helfrich authored the other preexisting course framework files carrying his name.");
      screen.bodyLines.push_back("- Mark Van Horn authored the new merge-shell and packaging files created in this workspace today.");
      screen.bodyLines.push_back("Integration notes:");
      screen.bodyLines.push_back("- Apollo uses the original flight, fuel, terrain, and landing logic.");
      screen.bodyLines.push_back("- Howitzer runs through a conflict-safe bridge scene with live rendering.");
      screen.bodyLines.push_back("- Chess uses an audited legacy rules core with room for AI and puzzle modes.");
      screen.bodyLines.push_back("- Orbital uses the legacy collision and debris model through a renamed bridge.");
      screen.bodyLines.push_back("Project status: macOS sampler build first, with shared shell and preview support in place.");
      screen.footerCommands = ShellCommandCatalog::forState(app.state());
      break;

   case AppState::InSimulator:
      screen.title = app.activeSceneName();
      screen.subtitle = app.activeSceneStatusText();
      screen.bodyLines = SceneStatusFormatter::format(app);

      if (app.controlsOverlayEnabled())
      {
         screen.bodyLines.push_back("Controls overlay");
         appendBindings(app, screen);
      }

      screen.footerCommands = ShellCommandCatalog::forState(app.state());
      break;

   case AppState::PauseMenu:
      screen.title = "Pause Menu";
      screen.subtitle = app.activeSceneName();
      screen.bodyLines =
      {
         "Simulation time is frozen.",
         "Resume returns to the active simulator immediately.",
         "Keybindings shows the current scene controls in a focused overlay.",
         "Quit returns to the main menu."
      };
      screen.footerCommands = ShellCommandCatalog::forState(app.state());
      break;

   case AppState::Keybindings:
      screen.title = "Keybindings";
      screen.subtitle = app.activeSceneName();
      screen.bodyLines.push_back("Scene controls:");
      appendBindings(app, screen);
      screen.bodyLines.push_back("Resume returns to the paused simulator.");
      screen.footerCommands = ShellCommandCatalog::forState(app.state());
      break;

   case AppState::ExitRequested:
      screen.title = "Exit";
      screen.subtitle = "Shutdown requested";
      screen.bodyLines = {"The application is closing."};
      break;
   }

   return screen;
}

void ShellPresenter::render(const ShellScreen & screen, std::ostream & output) const
{
   output << screen.title << "\n";
   if (!screen.subtitle.empty())
      output << screen.subtitle << "\n";

   for (const auto & line : screen.bodyLines)
      output << line << "\n";

   if (!screen.footerCommands.empty())
   {
      output << ShellTextFormatter::formatCommandFooter(screen.footerCommands) << "\n";
   }
}
