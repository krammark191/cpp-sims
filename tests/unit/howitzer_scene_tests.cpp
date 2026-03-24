#include <cassert>

#include "app/input_state.h"
#include "simulators/howitzer/howitzer_scene.h"

void runHowitzerSceneTests()
{
   HowitzerScene scene;

   assert(scene.id() == SimulatorId::Howitzer);
   assert(scene.isReady());
   assert(!scene.isPlaceholder());
   assert(scene.supportsRecordedPreview());
   assert(!scene.projectileActive());

   scene.enter();

   InputState aimUp;
   aimUp.upPressed = true;
   const double initialAngle = scene.elevationDegrees();
   scene.applyInput(aimUp);
   scene.update(0.5);
   assert(scene.elevationDegrees() != initialAngle);

   InputState fire;
   fire.firePressed = true;
   scene.applyInput(fire);
   scene.update(0.1);
   assert(scene.projectileActive());

   scene.applyInput(InputState{});
   scene.update(1.0);
   assert(scene.projectileAltitudeMeters() >= 0.0);

   for (int i = 0; i < 200 && scene.projectileActive(); ++i)
      scene.update(0.1);

   if (!scene.shotComplete())
   {
      scene.applyInput(fire);
      scene.update(0.1);
      assert(scene.projectileActive());
   }

   scene.exit();
   const double pausedElapsed = scene.elapsedSeconds();
   scene.update(1.0);
   assert(scene.elapsedSeconds() == pausedElapsed);

   ogstream gout;
   scene.render(gout, Position(1280.0, 720.0));
}
