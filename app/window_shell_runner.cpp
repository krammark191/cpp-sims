#include "app/window_shell_runner.h"

#include <algorithm>
#include <cstdlib>
#include <sstream>

#include "app/control_binding_formatter.h"
#include "app/preview_storyboard.h"
#include "app/scene_input_router.h"
#include "app/shell_command_catalog.h"
#include "app/shell_text_formatter.h"
#include "app/simulator_registry.h"
#include "simulators/chess/chess_scene.h"
#include "simulators/common/scene_render_utils.h"
#include "simulators/howitzer/howitzer_scene.h"
#include "simulators/lander/lander_scene.h"
#include "simulators/orbital/orbital_scene.h"

namespace
{
constexpr double kFrameStepSeconds = 1.0 / 30.0;

std::vector<std::string> playMenuCommands()
{
   return {"apollo", "howitzer", "chess", "orbital", "back"};
}

std::vector<std::string> wrapText(const std::string & text, std::size_t maxChars)
{
   if (text.empty() || text.length() <= maxChars)
      return {text};

   std::istringstream input(text);
   std::vector<std::string> lines;
   std::string current;
   std::string word;
   while (input >> word)
   {
      if (current.empty())
      {
         current = word;
         continue;
      }

      if (current.length() + 1 + word.length() <= maxChars)
      {
         current += ' ';
         current += word;
      }
      else
      {
         lines.push_back(current);
         current = word;
      }
   }

   if (!current.empty())
      lines.push_back(current);

   return lines;
}

std::vector<std::string> wrapLines(const std::vector<std::string> & lines, std::size_t maxChars)
{
   std::vector<std::string> wrapped;
   for (const auto & line : lines)
   {
      const auto split = wrapText(line, maxChars);
      wrapped.insert(wrapped.end(), split.begin(), split.end());
   }
   return wrapped;
}

std::string displayCommandName(AppState state, const std::string & command)
{
   if (command == "play")
      return "Play";
   if (command == "options")
      return "Options";
   if (command == "about")
      return "About";
   if (command == "quit")
      return state == AppState::MainMenu ? "Quit to Desktop" : "Quit to Main Menu";
   if (command == "resume")
      return "Resume";
   if (command == "keybindings")
      return "Keybindings";
   if (command == "back")
      return "Back";

   return command;
}

bool sceneHasCompletionOverlay(const Application & app)
{
   if (app.state() != AppState::InSimulator || app.activeScene() == nullptr)
      return false;

   if (const auto * lander = dynamic_cast<const LanderScene *>(app.activeScene()))
      return lander->hasLanded() || lander->hasCrashed();
   if (const auto * howitzer = dynamic_cast<const HowitzerScene *>(app.activeScene()))
      return howitzer->shotComplete();
   if (const auto * chess = dynamic_cast<const ChessScene *>(app.activeScene()))
      return chess->isGameOver();
   if (const auto * orbital = dynamic_cast<const OrbitalScene *>(app.activeScene()))
      return !orbital->dreamChaserAlive();

   return false;
}

std::vector<std::string> sceneCompletionLines(const Application & app)
{
   if (const auto * lander = dynamic_cast<const LanderScene *>(app.activeScene()))
      return {lander->hasLanded() ? "Landing successful." : "The lander crashed.",
              "Space: play again",
              "Esc: quit to main menu"};

   if (const auto * howitzer = dynamic_cast<const HowitzerScene *>(app.activeScene()))
      return {howitzer->lastShotHitTarget() ? "Target hit." : "Shot complete.",
              "Space: play again",
              "Esc: quit to main menu"};

   if (const auto * chess = dynamic_cast<const ChessScene *>(app.activeScene()))
      return {app.activeSceneStatusText(),
              "Space: play again",
              "Esc: quit to main menu"};

   if (const auto * orbital = dynamic_cast<const OrbitalScene *>(app.activeScene()))
      return {orbital->dreamChaserAlive() ? "Simulation complete." : "Dream Chaser destroyed.",
              "Space: play again",
              "Esc: quit to main menu"};

   return {"Space: play again", "Esc: quit to main menu"};
}

void drawShellBackdrop(AppState state, const Position & viewport, ogstream & gout)
{
   double red = 0.08;
   double green = 0.10;
   double blue = 0.14;

   switch (state)
   {
   case AppState::MainMenu:   red = 0.10; green = 0.10; blue = 0.14; break;
   case AppState::Options:    red = 0.08; green = 0.11; blue = 0.10; break;
   case AppState::About:      red = 0.11; green = 0.09; blue = 0.08; break;
   case AppState::PauseMenu:  red = 0.12; green = 0.09; blue = 0.08; break;
   case AppState::Keybindings:red = 0.08; green = 0.09; blue = 0.12; break;
   default: break;
   }

   SceneRenderUtils::drawPanel(gout, Position(0.0, 0.0), viewport, red, green, blue);
   SceneRenderUtils::drawPanel(gout,
                               Position(36.0, 36.0),
                               Position(viewport.getX() - 36.0, viewport.getY() - 36.0),
                               red + 0.03, green + 0.03, blue + 0.03);
   SceneRenderUtils::drawOutline(gout,
                                 Position(36.0, 36.0),
                                 Position(viewport.getX() - 36.0, viewport.getY() - 36.0),
                                 0.28, 0.32, 0.38);
   SceneRenderUtils::drawPanel(gout,
                               Position(56.0, viewport.getY() - 118.0),
                               Position(viewport.getX() - 56.0, viewport.getY() - 64.0),
                               0.16, 0.18, 0.22);
}
}

WindowShellRunner::WindowShellRunner() :
   app(),
   controller(),
   shellSession(),
   viewport(1280.0, 720.0),
   lastState(AppState::MainMenu),
   selectionIndex(0),
   lastNotice(),
   cachedPreviewId(),
   cachedPreviewStoryboard()
{
}

int WindowShellRunner::run()
{
   Interface ui("C++ Simulators", viewport);
   ui.run(callback, this);
   return 0;
}

void WindowShellRunner::callback(const Interface * pUI, void * p)
{
   auto * runner = static_cast<WindowShellRunner *>(p);
   runner->onFrame(pUI);
}

void WindowShellRunner::onFrame(const Interface * pUI)
{
   syncSelectionToState();

   if (app.state() == AppState::InSimulator)
      handleWindowSceneInput(pUI);
   else
   {
      handleWindowMenuInput(pUI);
      if (app.state() == AppState::PlayMenu)
         app.tick(kFrameStepSeconds);
   }

   ogstream gout;
   renderActiveFrame(gout);
   drawOverlay(gout);
}

void WindowShellRunner::renderActiveFrame(ogstream & gout) const
{
   if (app.state() == AppState::PlayMenu)
   {
      renderPlayMenu(gout);
      return;
   }

   if (app.state() == AppState::InSimulator)
   {
      if (const auto * lander = dynamic_cast<const LanderScene *>(app.activeScene()))
      {
         lander->render(gout);
         return;
      }

      if (const auto * howitzer = dynamic_cast<const HowitzerScene *>(app.activeScene()))
      {
         howitzer->render(gout, viewport);
         return;
      }

      if (const auto * chess = dynamic_cast<const ChessScene *>(app.activeScene()))
      {
         chess->render(gout, viewport);
         return;
      }

      if (const auto * orbital = dynamic_cast<const OrbitalScene *>(app.activeScene()))
      {
         orbital->render(gout, viewport);
         return;
      }
   }

   if (app.state() != AppState::InSimulator)
   {
      renderMenuScreen(gout);
      return;
   }

   shellSession.render(app, viewport, gout);
}

void WindowShellRunner::renderMenuScreen(ogstream & gout) const
{
   drawShellBackdrop(app.state(), viewport, gout);

   const ShellScreen screen = presenter.build(app);
   const auto commands = currentCommands();

   SceneRenderUtils::drawCenteredBanner(gout, viewport, viewport.getY() - 92.0, screen.title);
   if (!screen.subtitle.empty())
      SceneRenderUtils::drawCenteredBanner(gout, viewport, viewport.getY() - 126.0, screen.subtitle);

   if (app.state() == AppState::About)
   {
      std::vector<std::string> simulatorLines;
      for (const auto & simulator : SimulatorRegistry::all())
         simulatorLines.push_back(std::string(simulator.displayName) + ": " + simulator.summary);

      const double panelWidth = 460.0;
      const double gap = 20.0;
      const double left = (viewport.getX() - (panelWidth * 2.0 + gap)) * 0.5;
      SceneRenderUtils::drawInfoPanel(gout,
                                      Position(left, 250.0),
                                      Position(left + panelWidth, 560.0),
                                      "Project",
                                      wrapLines({
                                         "Four BYU-Idaho C++ simulators merged into one sampler shell.",
                                         "Current release target: macOS desktop app and dmg.",
                                         "Play opens recorded previews before launching a simulator.",
                                         "Menus, pause flow, and keybinding overlays are shared."
                                      }, 34),
                                      0.10, 0.12, 0.16);

      SceneRenderUtils::drawInfoPanel(gout,
                                      Position(left + panelWidth + gap, 250.0),
                                      Position(left + panelWidth * 2.0 + gap, 560.0),
                                      "Simulators",
                                      wrapLines(simulatorLines, 34),
                                      0.12, 0.14, 0.18);

      SceneRenderUtils::drawInfoPanel(gout,
                                      Position(left + 90.0, 132.0),
                                      Position(left + panelWidth * 2.0 + gap - 90.0, 226.0),
                                      "Authorship",
                                      {
                                         "Mark Van Horn and Taden Marston authored the preexisting files with their names.",
                                         "Dr. James Helfrich authored the other preexisting course framework files.",
                                         "Mark Van Horn authored the new merge-shell and packaging files in this workspace."
                                      },
                                      0.11, 0.13, 0.17);

      if (!commands.empty())
      {
         const Position bottomLeft((viewport.getX() - 280.0) * 0.5, 64.0);
         const Position topRight(bottomLeft.getX() + 280.0, 116.0);
         SceneRenderUtils::drawPanel(gout, bottomLeft, topRight, 0.22, 0.18, 0.08);
         SceneRenderUtils::drawOutline(gout, bottomLeft, topRight, 0.98, 0.82, 0.26);
         gout.setPosition(Position(bottomLeft.getX() + 28.0, 82.0));
         gout << "> Back";
      }

      SceneRenderUtils::drawCenteredBanner(gout,
                                           viewport,
                                           34.0,
                                           "Space returns to the previous menu.");
      return;
   }

   const double menuWidth = std::min(460.0, viewport.getX() * 0.46);
   const double menuLeft = (viewport.getX() - menuWidth) * 0.5;
   const double cardHeight = 56.0;
   const double cardGap = 18.0;
   const double commandCount = static_cast<double>(commands.size());
   const double stackHeight = commandCount * cardHeight + std::max(0.0, commandCount - 1.0) * cardGap;
   const double stackTop = viewport.getY() * 0.60 + stackHeight * 0.5;

   for (std::size_t i = 0; i < commands.size(); ++i)
   {
      const bool selected = static_cast<int>(i) == selectionIndex;
      const double top = stackTop - static_cast<double>(i) * (cardHeight + cardGap);
      const Position bottomLeft(menuLeft, top - cardHeight);
      const Position topRight(menuLeft + menuWidth, top);
      SceneRenderUtils::drawPanel(gout,
                                  bottomLeft,
                                  topRight,
                                  selected ? 0.22 : 0.11,
                                  selected ? 0.18 : 0.13,
                                  selected ? 0.08 : 0.16);
      SceneRenderUtils::drawOutline(gout,
                                    bottomLeft,
                                    topRight,
                                    selected ? 0.98 : 0.35,
                                    selected ? 0.82 : 0.38,
                                    selected ? 0.26 : 0.44);
      gout.setPosition(Position(menuLeft + 24.0, top - 34.0));
      gout << (selected ? "> " : "  ") << displayCommandName(app.state(), commands[i]);
   }

   if (!screen.bodyLines.empty())
   {
      const std::size_t maxLines = app.state() == AppState::MainMenu ? 4 : 8;
      const std::vector<std::string> lines(screen.bodyLines.begin(),
                                           screen.bodyLines.begin() +
                                              std::min(maxLines, screen.bodyLines.size()));
      SceneRenderUtils::drawInfoPanel(gout,
                                      Position(72.0, 82.0),
                                      Position(viewport.getX() - 72.0, 220.0),
                                      "Details",
                                      wrapLines(lines, 74),
                                      0.10, 0.12, 0.16);
   }

   SceneRenderUtils::drawCenteredBanner(gout,
                                        viewport,
                                        54.0,
                                        "Up/Down selects, Space confirms, Esc backs out.");
}

void WindowShellRunner::renderPlayMenu(ogstream & gout) const
{
   const auto * preview = app.selectedPreviewMetadata();
   if (preview == nullptr)
   {
      shellSession.render(app, viewport, gout);
      return;
   }

   const Position previewBottom(356.0, 304.0);
   const Position previewTop(viewport.getX() - 72.0, viewport.getY() - 250.0);
   if (const auto * storyboard = currentPreviewStoryboard())
   {
      previewRenderer.render(*storyboard,
                             app.previewPlaybackSeconds(),
                             previewBottom,
                             previewTop,
                             gout);
   }
   else
   {
      SceneRenderUtils::drawInfoPanel(gout,
                                      previewBottom,
                                      previewTop,
                                      "Preview Unavailable",
                                      wrapLines({
                                         "No storyboard manifest or video asset is present yet.",
                                         "Drop preview files into assets/demos and keep the registry paths.",
                                         "The menu loop timing and selection flow are already wired."
                                      }, 58),
                                      0.10, 0.12, 0.16);
   }

   SceneRenderUtils::drawInfoPanel(gout,
                                   Position(40.0, viewport.getY() - 248.0),
                                   Position(330.0, viewport.getY() - 40.0),
                                   "Play",
                                   wrapLines({
                                      std::string("Selected: ") + preview->displayName,
                                      std::string("Summary: ") + preview->summary,
                                      std::string("Preview source: ") +
                                         (app.previewStoryboardAvailable()
                                            ? preview->previewManifestPath
                                            : preview->demoVideoPath),
                                      std::string("Asset status: ") +
                                         (app.previewAssetAvailable()
                                            ? (app.previewStoryboardAvailable()
                                               ? "storyboard ready"
                                               : "video ready")
                                            : "missing"),
                                      "Loop: " +
                                         std::to_string(static_cast<int>(app.previewPlaybackSeconds())) +
                                         " / " +
                                         std::to_string(static_cast<int>(app.previewLoopDurationSeconds())) +
                                         " s",
                                      "Space opens the selected simulator."
                                   }, 30),
                                   0.10, 0.12, 0.16);

   const double cardLeft = 72.0;
   const double cardBottom = 84.0;
   const double cardWidth = 268.0;
   const double cardHeight = 92.0;
   const double cardGap = 16.0;
   const auto & simulators = SimulatorRegistry::all();
   for (std::size_t i = 0; i < simulators.size(); ++i)
   {
      const double left = cardLeft + i * (cardWidth + cardGap);
      const Position bottom(left, cardBottom);
      const Position top(left + cardWidth, cardBottom + cardHeight);
      const bool selected = i == app.playMenuSelectionIndex();

      SceneRenderUtils::drawPanel(gout,
                                  bottom,
                                  top,
                                  selected ? 0.18 : 0.09,
                                  selected ? 0.20 : 0.11,
                                  selected ? 0.12 : 0.15);
      SceneRenderUtils::drawOutline(gout,
                                    bottom,
                                    top,
                                    selected ? 0.95 : 0.28,
                                    selected ? 0.84 : 0.30,
                                    selected ? 0.24 : 0.34);

      gout.setPosition(Position(left + 16.0, cardBottom + 62.0));
      gout << simulators[i].displayName;
      gout.setPosition(Position(left + 16.0, cardBottom + 42.0));
      gout << (simulators[i].supportsRecordedPreview ? "Preview path wired" : "No preview support");
      gout.setPosition(Position(left + 16.0, cardBottom + 22.0));
      gout << simulators[i].sceneClassName;
   }
}

void WindowShellRunner::handleWindowMenuInput(const Interface * pUI)
{
   const auto commands = currentCommands();
   if (commands.empty())
      return;

   if (app.state() == AppState::PlayMenu)
   {
      if (pUI->isLeft() == 1 || pUI->isUp() == 1)
      {
         app.selectPreviousPreview();
         selectionIndex = static_cast<int>(app.playMenuSelectionIndex());
      }
      if (pUI->isRight() == 1 || pUI->isDown() == 1)
      {
         app.selectNextPreview();
         selectionIndex = static_cast<int>(app.playMenuSelectionIndex());
      }

      if (pUI->isSpace())
         submitCommand("open");

      if (pUI->isEscape() || pUI->isQ())
         submitCommand("back");
      return;
   }

   if (pUI->isUp() == 1)
      selectionIndex = (selectionIndex + static_cast<int>(commands.size()) - 1) % static_cast<int>(commands.size());
   if (pUI->isDown() == 1)
      selectionIndex = (selectionIndex + 1) % static_cast<int>(commands.size());

   if (pUI->isSpace())
      submitCommand(commands[selectionIndex]);

   if (pUI->isEscape() || pUI->isQ())
   {
      if (app.state() == AppState::MainMenu)
         submitCommand("quit");
      else if (app.state() == AppState::PauseMenu)
         submitCommand("resume");
      else if (app.state() == AppState::Keybindings)
         submitCommand("resume");
      else
         submitCommand("back");
   }
}

void WindowShellRunner::handleWindowSceneInput(const Interface * pUI)
{
   if (sceneHasCompletionOverlay(app))
   {
      app.setInputState(InputState{});
      app.tick(kFrameStepSeconds);
      app.clearInputState();

      if (pUI->isSpace())
         submitCommand("reset");
      else if (pUI->isEscape() || pUI->isQ())
         submitCommand("back");
      return;
   }

   if (pUI->isEscape() || pUI->isQ())
   {
      submitCommand("pause");
      return;
   }

   const auto id = app.selectedSimulator();
   if (!id.has_value())
      return;

   const InputState input = SceneInputRouter::fromRealtimeControls(*id,
                                                                   pUI->isLeft() != 0,
                                                                   pUI->isRight() != 0,
                                                                   pUI->isUp() != 0,
                                                                   pUI->isDown() != 0,
                                                                   pUI->isSpace());

   app.setInputState(input);
   app.tick(kFrameStepSeconds);
   app.clearInputState();

   if (pUI->isSpace() && SceneInputRouter::opensKeybindingsOnSpace(*id))
      submitCommand("keybindings");
}

void WindowShellRunner::syncSelectionToState()
{
   if (app.state() == lastState)
   {
      if (app.state() == AppState::PlayMenu)
         selectionIndex = static_cast<int>(app.playMenuSelectionIndex());
      return;
   }

   selectionIndex = 0;
   if (app.state() == AppState::PlayMenu)
      selectionIndex = static_cast<int>(app.playMenuSelectionIndex());
   lastState = app.state();
}

std::vector<std::string> WindowShellRunner::currentCommands() const
{
   if (app.state() == AppState::PlayMenu)
      return playMenuCommands();

   return ShellCommandCatalog::forState(app.state());
}

void WindowShellRunner::submitCommand(const std::string & command)
{
   const auto result = controller.handleCommand(app, command);
   if (!result.notices.empty())
      lastNotice = result.notices.back();

   if (!result.continueRunning)
      std::exit(0);
}

void WindowShellRunner::drawOverlay(ogstream & gout) const
{
   if (app.state() == AppState::PauseMenu || app.state() == AppState::Keybindings)
   {
      SceneRenderUtils::drawPanel(gout,
                                  Position(24.0, 24.0),
                                  Position(viewport.getX() - 24.0, 72.0),
                                  0.14, 0.12, 0.10);
      SceneRenderUtils::drawOutline(gout,
                                    Position(24.0, 24.0),
                                    Position(viewport.getX() - 24.0, 72.0),
                                    0.40, 0.34, 0.24);
   }

   if (app.state() == AppState::InSimulator && app.controlsOverlayEnabled())
   {
      const auto lines = ControlBindingFormatter::formatAll(app.activeSceneKeybindings());
      const double panelHeight = 24.0 + static_cast<double>(lines.size()) * 18.0;
      const double panelWidth = 360.0;
      const double left = (viewport.getX() - panelWidth) * 0.5;
      SceneRenderUtils::drawPanel(gout,
                                  Position(left, viewport.getY() - panelHeight - 20.0),
                                  Position(left + panelWidth, viewport.getY() - 18.0),
                                  0.08, 0.09, 0.11);
      SceneRenderUtils::drawOutline(gout,
                                    Position(left, viewport.getY() - panelHeight - 20.0),
                                    Position(left + panelWidth, viewport.getY() - 18.0),
                                    0.28, 0.30, 0.34);
      double y = viewport.getY() - 40.0;
      for (const auto & line : lines)
      {
         gout.setPosition(Position(left + 18.0, y));
         gout << line;
         y -= 18.0;
      }
   }

   if (sceneHasCompletionOverlay(app))
   {
      SceneRenderUtils::drawInfoPanel(gout,
                                      Position((viewport.getX() - 360.0) * 0.5, 86.0),
                                      Position((viewport.getX() + 360.0) * 0.5, 202.0),
                                      "Run Complete",
                                      sceneCompletionLines(app),
                                      0.16, 0.14, 0.10);
   }

   const auto commands = currentCommands();
   if (app.state() == AppState::PlayMenu &&
       !commands.empty() &&
       selectionIndex >= 0 &&
       selectionIndex < static_cast<int>(commands.size()))
   {
      gout.setPosition(Position(32.0, 52.0));
      const auto * preview = app.selectedPreviewMetadata();
      if (preview != nullptr)
         gout << "Preview focus: " << preview->displayName;
   }

   if (!lastNotice.empty())
   {
      gout.setPosition(Position(32.0, 28.0));
      gout << lastNotice;
   }
}

const PreviewStoryboard * WindowShellRunner::currentPreviewStoryboard() const
{
   const auto * metadata = app.selectedPreviewMetadata();
   if (metadata == nullptr)
      return nullptr;

   if (!cachedPreviewId.has_value() || *cachedPreviewId != metadata->id)
   {
      cachedPreviewId = metadata->id;
      cachedPreviewStoryboard = PreviewStoryboardLoader::load(*metadata);
   }

   return cachedPreviewStoryboard ? &*cachedPreviewStoryboard : nullptr;
}
