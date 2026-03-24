#include "app/application.h"

#include <filesystem>

#include "app/preview_storyboard.h"
#include "app/runtime_paths.h"
#include "app/scene_factory.h"
#include "app/simulator_registry.h"

namespace
{
const std::vector<ControlBinding> kEmptyBindings;

std::size_t firstPreviewIndex()
{
   return 0;
}

double resolvePreviewLoopSeconds(const SimulatorMetadata * metadata)
{
   if (metadata == nullptr)
      return 12.0;

   const auto storyboard = PreviewStoryboardLoader::load(*metadata);
   if (!storyboard.has_value() || storyboard->frames.empty())
      return 12.0;

   return storyboard->frameSeconds * static_cast<double>(storyboard->frames.size());
}
}

Application::Application() :
   currentState(AppState::MainMenu),
   currentSimulator(),
   showControlsOverlay(true),
   controlsOverlaySeconds(5),
   audioSettingsHooksReady(true),
   currentInput(),
   previewSelectionIndex(firstPreviewIndex()),
   previewSeconds(0.0),
   previewLoopSeconds(12.0),
   scene()
{
}

AppState Application::state() const
{
   return currentState;
}

std::optional<SimulatorId> Application::selectedSimulator() const
{
   return currentSimulator;
}

bool Application::controlsOverlayEnabled() const
{
   return showControlsOverlay;
}

int Application::controlsOverlayDurationSeconds() const
{
   return controlsOverlaySeconds;
}

bool Application::audioHooksEnabled() const
{
   return audioSettingsHooksReady;
}

bool Application::quitRequested() const
{
   return currentState == AppState::ExitRequested;
}

bool Application::hasActiveScene() const
{
   return static_cast<bool>(scene);
}

const ISimulatorScene * Application::activeScene() const
{
   return scene.get();
}

const char * Application::activeSceneName() const
{
   return scene ? scene->sceneName() : "";
}

const char * Application::activeSceneStatusText() const
{
   return scene ? scene->statusText() : "";
}

const std::vector<ControlBinding> & Application::activeSceneKeybindings() const
{
   return scene ? scene->keybindings() : kEmptyBindings;
}

double Application::activeSceneElapsedSeconds() const
{
   return scene ? scene->elapsedSeconds() : 0.0;
}

const SimulatorMetadata * Application::selectedPreviewMetadata() const
{
   const auto & simulators = SimulatorRegistry::all();
   if (simulators.empty() || previewSelectionIndex >= simulators.size())
      return nullptr;

   return &simulators[previewSelectionIndex];
}

std::size_t Application::playMenuSelectionIndex() const
{
   return previewSelectionIndex;
}

double Application::previewPlaybackSeconds() const
{
   return previewSeconds;
}

double Application::previewLoopDurationSeconds() const
{
   return previewLoopSeconds;
}

bool Application::previewAssetAvailable() const
{
   const auto * metadata = selectedPreviewMetadata();
   if (metadata == nullptr || metadata->demoVideoPath == nullptr)
      return false;

   return std::filesystem::exists(RuntimePaths::resolveRelativePath(metadata->demoVideoPath)) ||
      PreviewStoryboardLoader::hasStoryboard(*metadata);
}

bool Application::previewStoryboardAvailable() const
{
   const auto * metadata = selectedPreviewMetadata();
   return metadata != nullptr && PreviewStoryboardLoader::hasStoryboard(*metadata);
}

void Application::openPlayMenu()
{
   currentState = AppState::PlayMenu;
   previewSelectionIndex = firstPreviewIndex();
   previewSeconds = 0.0;
   previewLoopSeconds = resolvePreviewLoopSeconds(selectedPreviewMetadata());
}

void Application::openOptions()
{
   currentState = AppState::Options;
}

void Application::openAbout()
{
   currentState = AppState::About;
}

bool Application::openSimulator(SimulatorId id)
{
   auto nextScene = SceneFactory::create(id);
   if (!nextScene)
      return false;

   if (scene)
      scene->exit();

   nextScene->reset();
   nextScene->enter();
   scene = std::move(nextScene);
   currentSimulator = id;
   currentState = AppState::InSimulator;
   showControlsOverlay = true;
   selectPreview(id);
   return true;
}

bool Application::openPauseMenu()
{
   if (currentState != AppState::InSimulator)
      return false;

   currentState = AppState::PauseMenu;
   return true;
}

bool Application::resumeSimulator()
{
   if (currentState != AppState::PauseMenu &&
       currentState != AppState::Keybindings)
      return false;

   currentState = AppState::InSimulator;
   return true;
}

bool Application::openKeybindings()
{
   if (currentState != AppState::PauseMenu)
      return false;

   currentState = AppState::Keybindings;
   return true;
}

bool Application::quitSimulatorToMenu()
{
   if (currentState != AppState::PauseMenu &&
       currentState != AppState::Keybindings &&
       currentState != AppState::InSimulator)
      return false;

   if (scene)
   {
      scene->exit();
      scene.reset();
   }

   currentSimulator.reset();
   currentState = AppState::MainMenu;
   return true;
}

bool Application::selectPreview(SimulatorId id)
{
   const auto & simulators = SimulatorRegistry::all();
   for (std::size_t i = 0; i < simulators.size(); ++i)
   {
      if (simulators[i].id != id)
         continue;

      previewSelectionIndex = i;
      previewSeconds = 0.0;
      previewLoopSeconds = resolvePreviewLoopSeconds(&simulators[i]);
      return true;
   }

   return false;
}

bool Application::selectNextPreview()
{
   const auto & simulators = SimulatorRegistry::all();
   if (simulators.empty())
      return false;

   previewSelectionIndex = (previewSelectionIndex + 1) % simulators.size();
   previewSeconds = 0.0;
   previewLoopSeconds = resolvePreviewLoopSeconds(&simulators[previewSelectionIndex]);
   return true;
}

bool Application::selectPreviousPreview()
{
   const auto & simulators = SimulatorRegistry::all();
   if (simulators.empty())
      return false;

   previewSelectionIndex =
      (previewSelectionIndex + simulators.size() - 1) % simulators.size();
   previewSeconds = 0.0;
   previewLoopSeconds = resolvePreviewLoopSeconds(&simulators[previewSelectionIndex]);
   return true;
}

bool Application::openSelectedPreview()
{
   const auto * metadata = selectedPreviewMetadata();
   if (metadata == nullptr)
      return false;

   return openSimulator(metadata->id);
}

void Application::returnToMainMenu()
{
   if (scene)
   {
      scene->exit();
      scene.reset();
   }

   currentSimulator.reset();
   currentState = AppState::MainMenu;
}

void Application::requestQuit()
{
   if (scene)
   {
      scene->exit();
      scene.reset();
   }

   currentSimulator.reset();

   currentState = AppState::ExitRequested;
}

void Application::setInputState(const InputState & input)
{
   currentInput = input;
}

void Application::clearInputState()
{
   currentInput = InputState{};
}

void Application::tick(double dtSeconds)
{
   if (currentState == AppState::PlayMenu)
   {
      previewSeconds += dtSeconds;
      while (previewSeconds >= previewLoopSeconds)
         previewSeconds -= previewLoopSeconds;
      return;
   }

   if (currentState != AppState::InSimulator || !scene)
      return;

   scene->applyInput(currentInput);
   scene->update(dtSeconds);
   showControlsOverlay = scene->elapsedSeconds() < controlsOverlaySeconds;
}
