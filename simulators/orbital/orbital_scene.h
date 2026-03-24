#pragma once

#include <memory>
#include <vector>

#include "app/isimulator_scene.h"
#include "simulators/lander/legacy/uiDraw.h"

class OrbitalScene : public ISimulatorScene
{
public:
   OrbitalScene();
   ~OrbitalScene();

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

   void render(ogstream & gout, const Position & viewport) const;
   std::size_t liveObjectCount() const;
   std::size_t fragmentCount() const;
   std::size_t projectileCount() const;
   bool dreamChaserAlive() const;

private:
   struct Impl;

   std::vector<ControlBinding> bindings;
   std::unique_ptr<Impl> impl;
};
