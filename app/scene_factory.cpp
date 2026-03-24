#include "app/scene_factory.h"

#include "simulators/chess/chess_scene.h"
#include "simulators/howitzer/howitzer_scene.h"
#include "simulators/common/placeholder_scene.h"
#include "simulators/lander/lander_scene.h"
#include "simulators/orbital/orbital_scene.h"

std::unique_ptr<ISimulatorScene> SceneFactory::create(SimulatorId id)
{
   switch (id)
   {
   case SimulatorId::ApolloLander:
      return std::make_unique<LanderScene>();

   case SimulatorId::Howitzer:
      return std::make_unique<HowitzerScene>();

   case SimulatorId::Chess:
      return std::make_unique<ChessScene>();

   case SimulatorId::Orbital:
      return std::make_unique<OrbitalScene>();
   }

   return nullptr;
}
