#pragma once

#include <vector>

#include "app/simulator_metadata.h"

class SimulatorRegistry
{
public:
   static const std::vector<SimulatorMetadata> & all();
   static const SimulatorMetadata * find(SimulatorId id);
};
