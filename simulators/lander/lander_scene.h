#pragma once

#include <memory>
#include <vector>

#include "app/isimulator_scene.h"
#include "simulators/lander/legacy/uiDraw.h"

class LanderScene : public ISimulatorScene
{
public:
   LanderScene();
   ~LanderScene();

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

   bool isActive() const;
   double demoAltitudeMeters() const;
   double demoFuelPounds() const;
   double demoVelocityMetersPerSecond() const;
   bool isFlying() const;
   bool hasLanded() const;
   bool hasCrashed() const;
   bool hasCrashEffects() const;
   void render(ogstream & gout) const;

private:
   struct Impl;

   std::vector<ControlBinding> bindings;
   std::unique_ptr<Impl> impl;
};
