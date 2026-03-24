#include <cassert>
#include <string>

#include "app/input_state.h"
#include "simulators/orbital/orbital_scene.h"

void runOrbitalSceneTests()
{
   {
      OrbitalScene scene;

      scene.reset();
      scene.enter();

      assert(scene.id() == SimulatorId::Orbital);
      assert(scene.isReady());
      assert(!scene.isPlaceholder());
      assert(scene.supportsRecordedPreview());
      assert(!scene.supportsFutureAiModes());
      assert(scene.liveObjectCount() == 11);
      assert(scene.fragmentCount() == 0);
      assert(scene.projectileCount() == 0);
   }

   {
      OrbitalScene scene;
      InputState input;

      scene.reset();
      scene.enter();

      input.mainThrust = true;
      input.firePressed = true;
      scene.applyInput(input);
      scene.update(0.1);

      assert(scene.elapsedSeconds() > 0.0);
      assert(scene.projectileCount() >= 1);
      assert(std::string(scene.statusText()).find("Dream Chaser") != std::string::npos);
   }

   {
      OrbitalScene scene;
      ogstream gout;

      scene.reset();
      scene.enter();
      scene.render(gout, Position(1280.0, 720.0));
   }
}
