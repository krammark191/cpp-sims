#pragma once

#include <memory>
#include <vector>

#include "app/isimulator_scene.h"
#include "simulators/lander/legacy/uiDraw.h"

class ChessScene : public ISimulatorScene
{
public:
   struct Impl;

   ChessScene();
   ~ChessScene();

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
   bool hasPendingPromotion() const;
   bool isGameOver() const;

private:
   std::vector<ControlBinding> bindings;
   std::unique_ptr<Impl> impl;
};
