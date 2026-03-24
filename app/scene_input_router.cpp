#include "app/scene_input_router.h"

namespace SceneInputRouter
{
InputState fromStepKeys(SimulatorId id, const std::string & keys)
{
   InputState input;

   for (char key : keys)
   {
      switch (key)
      {
      case 'a':
         input.rotateLeft = true;
         break;
      case 'd':
         input.rotateRight = true;
         break;
      case 'w':
         if (id == SimulatorId::ApolloLander || id == SimulatorId::Orbital)
            input.mainThrust = true;
         else
            input.upPressed = true;
         break;
      case 's':
         input.downPressed = true;
         break;
      case 'f':
         input.firePressed = true;
         break;
      default:
         break;
      }
   }

   return input;
}

InputState fromRealtimeControls(SimulatorId id,
                                bool leftPressed,
                                bool rightPressed,
                                bool upPressed,
                                bool downPressed,
                                bool spacePressed)
{
   InputState input;

   switch (id)
   {
   case SimulatorId::ApolloLander:
      input.mainThrust = downPressed;
      input.rotateLeft = leftPressed;
      input.rotateRight = rightPressed;
      break;

   case SimulatorId::Howitzer:
      input.rotateLeft = leftPressed;
      input.rotateRight = rightPressed;
      input.upPressed = upPressed;
      input.downPressed = downPressed;
      input.firePressed = spacePressed;
      break;

   case SimulatorId::Chess:
      input.rotateLeft = leftPressed;
      input.rotateRight = rightPressed;
      input.upPressed = upPressed;
      input.downPressed = downPressed;
      input.firePressed = spacePressed;
      input.selectPressed = spacePressed;
      break;

   case SimulatorId::Orbital:
      input.rotateLeft = leftPressed;
      input.rotateRight = rightPressed;
      input.mainThrust = downPressed;
      input.downPressed = downPressed;
      input.firePressed = spacePressed;
      break;
   }

   return input;
}

bool opensKeybindingsOnSpace(SimulatorId id)
{
   return id != SimulatorId::Howitzer &&
          id != SimulatorId::Chess &&
          id != SimulatorId::Orbital;
}
}
