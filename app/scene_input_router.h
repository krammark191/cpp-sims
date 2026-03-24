#pragma once

#include <string>

#include "app/input_state.h"
#include "app/simulator_id.h"

namespace SceneInputRouter
{
InputState fromStepKeys(SimulatorId id, const std::string & keys);

InputState fromRealtimeControls(SimulatorId id,
                                bool leftPressed,
                                bool rightPressed,
                                bool upPressed,
                                bool downPressed,
                                bool spacePressed);

bool opensKeybindingsOnSpace(SimulatorId id);
}
