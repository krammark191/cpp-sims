#pragma once

#include <vector>

#include "app/isimulator_scene.h"

class PlaceholderScene : public ISimulatorScene
{
public:
   PlaceholderScene(SimulatorId id, const char * sceneName, const char * statusText);

   SimulatorId id() const override;
   const char * sceneName() const override;
   const char * statusText() const override;
   const std::vector<ControlBinding> & keybindings() const override;
   bool supportsRecordedPreview() const override;
   bool supportsFutureAiModes() const override;
   bool isReady() const override;
   bool isPlaceholder() const override;
   void enter() override;
   void exit() override;
   void reset() override;
   void applyInput(const InputState & input) override;
   void update(double dtSeconds) override;
   double elapsedSeconds() const override;

private:
   SimulatorId simulatorId;
   const char * name;
   const char * status;
   std::vector<ControlBinding> bindings;
   bool active;
   double secondsSinceEnter;
};
