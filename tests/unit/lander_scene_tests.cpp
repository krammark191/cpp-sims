#include <cassert>

#include "app/input_state.h"
#include "simulators/lander/lander_scene.h"

void runLanderSceneTests()
{
   LanderScene scene;

   assert(scene.id() == SimulatorId::ApolloLander);
   assert(scene.isReady());
   assert(!scene.isPlaceholder());
   assert(scene.supportsRecordedPreview());
   assert(!scene.supportsFutureAiModes());
   assert(scene.demoFuelPounds() > 0.0);

   scene.enter();
   scene.applyInput(InputState{});

   const double initialAltitude = scene.demoAltitudeMeters();
   const double initialFuel = scene.demoFuelPounds();
   const double initialVelocity = scene.demoVelocityMetersPerSecond();

   scene.update(1.0);

   assert(scene.elapsedSeconds() > 0.0);
   assert(scene.demoAltitudeMeters() != initialAltitude);
   assert(scene.demoFuelPounds() == initialFuel);
   assert(scene.demoVelocityMetersPerSecond() != initialVelocity);

   InputState thrustInput;
   thrustInput.mainThrust = true;
   scene.applyInput(thrustInput);
   scene.update(1.0);
   assert(scene.demoFuelPounds() < initialFuel);

   scene.exit();
   const double pausedElapsed = scene.elapsedSeconds();
   scene.update(1.0);
   assert(scene.elapsedSeconds() == pausedElapsed);

   scene.reset();
   assert(scene.elapsedSeconds() == 0.0);
   assert(scene.isFlying());

   scene.enter();
   scene.applyInput(InputState{});
   for (int i = 0; i < 600 && scene.isFlying(); ++i)
      scene.update(0.1);
   assert(!scene.isFlying());
   assert(scene.hasCrashed() || scene.hasLanded());
   if (scene.hasCrashed())
      assert(scene.hasCrashEffects());

   ogstream gout;
   scene.render(gout);
}
