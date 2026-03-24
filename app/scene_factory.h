#pragma once

#include <memory>

#include "app/isimulator_scene.h"
#include "app/simulator_id.h"

class SceneFactory
{
public:
   static std::unique_ptr<ISimulatorScene> create(SimulatorId id);
};
