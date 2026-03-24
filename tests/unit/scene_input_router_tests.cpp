#include <cassert>

#include "app/scene_input_router.h"

void runSceneInputRouterTests()
{
   {
      const InputState input = SceneInputRouter::fromStepKeys(SimulatorId::ApolloLander, "wad");
      assert(input.mainThrust);
      assert(input.rotateLeft);
      assert(input.rotateRight);
      assert(!input.upPressed);
   }

   {
      const InputState input = SceneInputRouter::fromStepKeys(SimulatorId::Howitzer, "wsf");
      assert(input.upPressed);
      assert(input.downPressed);
      assert(input.firePressed);
      assert(!input.mainThrust);
   }

   {
      const InputState input = SceneInputRouter::fromRealtimeControls(SimulatorId::Chess,
                                                                      true, false, true, false, true);
      assert(input.rotateLeft);
      assert(input.upPressed);
      assert(input.firePressed);
      assert(input.selectPressed);
   }

   {
      const InputState input = SceneInputRouter::fromRealtimeControls(SimulatorId::Orbital,
                                                                      true, true, false, true, true);
      assert(input.rotateLeft);
      assert(input.rotateRight);
      assert(input.mainThrust);
      assert(input.downPressed);
      assert(input.firePressed);
      assert(!input.selectPressed);
   }

   {
      assert(SceneInputRouter::opensKeybindingsOnSpace(SimulatorId::ApolloLander));
      assert(!SceneInputRouter::opensKeybindingsOnSpace(SimulatorId::Howitzer));
      assert(!SceneInputRouter::opensKeybindingsOnSpace(SimulatorId::Chess));
      assert(!SceneInputRouter::opensKeybindingsOnSpace(SimulatorId::Orbital));
   }
}
