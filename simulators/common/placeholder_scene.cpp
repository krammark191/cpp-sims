#include "simulators/common/placeholder_scene.h"

PlaceholderScene::PlaceholderScene(SimulatorId id,
                                   const char * sceneName,
                                   const char * statusText) :
   simulatorId(id),
   name(sceneName),
   status(statusText),
   bindings(
   {
      {"Pause menu", "Esc", nullptr},
      {"Resume", "Enter", nullptr},
      {"Quit to main menu", "Q", nullptr}
   }),
   active(false),
   secondsSinceEnter(0.0)
{
}

SimulatorId PlaceholderScene::id() const
{
   return simulatorId;
}

const char * PlaceholderScene::sceneName() const
{
   return name;
}

const char * PlaceholderScene::statusText() const
{
   return status;
}

const std::vector<ControlBinding> & PlaceholderScene::keybindings() const
{
   return bindings;
}

bool PlaceholderScene::supportsRecordedPreview() const
{
   return true;
}

bool PlaceholderScene::supportsFutureAiModes() const
{
   return simulatorId == SimulatorId::Chess;
}

bool PlaceholderScene::isReady() const
{
   return false;
}

bool PlaceholderScene::isPlaceholder() const
{
   return true;
}

void PlaceholderScene::enter()
{
   active = true;
}

void PlaceholderScene::exit()
{
   active = false;
}

void PlaceholderScene::reset()
{
   secondsSinceEnter = 0.0;
}

void PlaceholderScene::applyInput(const InputState &)
{
}

void PlaceholderScene::update(double dtSeconds)
{
   if (!active)
      return;

   secondsSinceEnter += dtSeconds;
}

double PlaceholderScene::elapsedSeconds() const
{
   return secondsSinceEnter;
}
