#pragma once

#include <vector>

#include "app/control_binding.h"
#include "app/input_state.h"
#include "app/simulator_id.h"

class ISimulatorScene
{
public:
   virtual ~ISimulatorScene() = default;

   virtual SimulatorId id() const = 0;
   virtual const char * sceneName() const = 0;
   virtual const char * statusText() const = 0;
   virtual const std::vector<ControlBinding> & keybindings() const = 0;
   virtual bool supportsRecordedPreview() const = 0;
   virtual bool supportsFutureAiModes() const = 0;
   virtual bool isReady() const = 0;
   virtual bool isPlaceholder() const = 0;
   virtual void enter() = 0;
   virtual void exit() = 0;
   virtual void reset() = 0;
   virtual void applyInput(const InputState & input) = 0;
   virtual void update(double dtSeconds) = 0;
   virtual double elapsedSeconds() const = 0;
};
