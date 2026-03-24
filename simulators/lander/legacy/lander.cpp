/***********************************************************************
 * Source File:
 *    LANDER
 * Author:
 *    Br. Helfrich
 * Summary:
 *    All the information about the lunar lander
 ************************************************************************/

#include "lander.h"
#include "acceleration.h"

/***************************************************************
 * RESET
 * Reset the lander and its position to start the game over
 ***************************************************************/
void Lander :: reset(const Position & posUpperRight)
{
   angle.setUp();
   status = PLAYING;
   fuel = 5000.0;
   velocity.setDX(random(-10, -4));
   velocity.setDY(random(-2, 2));
   pos.setX(posUpperRight.getX() - 1);
   pos.setY(posUpperRight.getY() * random(0.75, 0.95));
}

/***************************************************************
 * DRAW
 * Draw the lander on the screen
 ***************************************************************/
void Lander :: draw(const Thrust & thrust, ogstream & gout) const
{
   // Draw the main body of the lander
   gout.drawLander(pos, angle.getRadians());
   
   // Determine which flames to draw based on the thrust input
   const bool hasFuel = fuel > 0.0;
   bool mainEngine = thrust.isMain() && hasFuel;
   bool leftThrust = thrust.isCounter() && hasFuel;
   bool rightThrust = thrust.isClock() && hasFuel;
   
   // Draw the flames under the lander
   if (isFlying())
   {
      gout.drawLanderFlames(pos, angle.getRadians(),
         mainEngine,
         leftThrust,
         rightThrust);
   }
}

/***************************************************************
 * INPUT
 * Accept input from the Neil Armstrong
 ***************************************************************/
Acceleration Lander::input(const Thrust &thrust, double gravity)
{
   // Initialize acceleration
   Acceleration acceleration;
   
   // Calculate thrust acceleration
   double thrustAccel = (fuel > 0) ? thrust.mainEngineThrust() : 0.0;
   
   // Calculate the horizontal and vertical components of the thrust acceleration
   double thrustX = thrustAccel * -sin(angle.getRadians());
   double thrustY = thrustAccel * cos(angle.getRadians());
   
   // Calculate the total acceleration considering thrust and gravity
   acceleration.setDDX(thrustX);
   acceleration.setDDY(thrustY + gravity);
   
   // Adjust fuel based on thrust usage
   if (thrust.mainEngineThrust() && fuel > 0)
   {
      fuel -= 8;
      if (fuel < 0.0)
         fuel = 0.0;
   }
   
   // Handle rotation
   if (thrust.isClock() && fuel > 0)
   {
      angle.add(0.1);
      fuel -= 0.8;
      if (fuel < 0.0)
         fuel = 0.0;
   }
   else if (thrust.isCounter() && fuel > 0)
   {
      angle.add(-0.1);
      fuel -= 0.8;
      if (fuel < 0.0)
         fuel = 0.0;
   }
   
   return acceleration;
}

/******************************************************************
 * COAST
 * What happens when we coast?
 *******************************************************************/
void Lander :: coast(Acceleration & acceleration, double time)
{
   pos.add(acceleration, velocity, time);
   velocity.add(acceleration, time);
}
