#include "app/simulator_registry.h"

namespace
{
const std::vector<SimulatorMetadata> kSimulators =
{
   {
      SimulatorId::ApolloLander,
      "Apollo 11 Lander",
      "A lunar landing simulator focused on fuel, thrust, and touchdown control.",
      "assets/demos/apollo11_lander_loop.mp4",
      "assets/demos/apollo11_lander_loop.preview.txt",
      "LanderScene",
      true,
      false
   },
   {
      SimulatorId::Howitzer,
      "Howitzer Artillery",
      "A ballistic artillery simulator with terrain, launch angle, and impact logic.",
      "assets/demos/howitzer_loop.mp4",
      "assets/demos/howitzer_loop.preview.txt",
      "HowitzerScene",
      true,
      false
   },
   {
      SimulatorId::Chess,
      "Chess",
      "A local two-player chess game that will be audited against official rules.",
      "assets/demos/chess_loop.mp4",
      "assets/demos/chess_loop.preview.txt",
      "ChessScene",
      true,
      true
   },
   {
      SimulatorId::Orbital,
      "Orbital Simulator",
      "A collision-heavy orbital sandbox with satellites, debris, and spacecraft.",
      "assets/demos/orbital_loop.mp4",
      "assets/demos/orbital_loop.preview.txt",
      "OrbitalScene",
      true,
      false
   }
};
}

const std::vector<SimulatorMetadata> & SimulatorRegistry::all()
{
   return kSimulators;
}

const SimulatorMetadata * SimulatorRegistry::find(SimulatorId id)
{
   for (const auto & simulator : kSimulators)
   {
      if (simulator.id == id)
         return &simulator;
   }

   return nullptr;
}
