#include <cassert>

#include "app/simulator_registry.h"

void runSimulatorRegistryTests()
{
   const auto & simulators = SimulatorRegistry::all();

   assert(simulators.size() == 4);

   for (const auto & simulator : simulators)
   {
      assert(simulator.displayName != nullptr);
      assert(simulator.summary != nullptr);
      assert(simulator.demoVideoPath != nullptr);
      assert(simulator.previewManifestPath != nullptr);
      assert(simulator.sceneClassName != nullptr);
      assert(simulator.supportsRecordedPreview);
   }

   const auto * chess = SimulatorRegistry::find(SimulatorId::Chess);
   assert(chess != nullptr);
   assert(chess->supportsFutureAiModes);

   const auto * apollo = SimulatorRegistry::find(SimulatorId::ApolloLander);
   assert(apollo != nullptr);
   assert(apollo->supportsRecordedPreview);

   const auto * orbital = SimulatorRegistry::find(SimulatorId::Orbital);
   assert(orbital != nullptr);
   assert(!orbital->supportsFutureAiModes);

   const auto * howitzer = SimulatorRegistry::find(SimulatorId::Howitzer);
   assert(howitzer != nullptr);
   assert(howitzer->supportsRecordedPreview);
}
