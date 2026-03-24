/***********************************************************************
 * Source File:
 *    FRAGMENT
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Implementation of the fragment debris class
 ************************************************************************/

#include "fragment.h"
#include "uiDraw.h"
#include "physics.h"
#include <cmath>

 /*********************************************
  * FRAGMENT : UPDATE
  * Updates the fragment's position and velocity
  * based on gravity and time, increments lifetime.
  *********************************************/
void Fragment::update(double time)
{
   if (alive)
   {
      // Move fragment
      Acceleration gravity = getGravity(position);
      updateVelocity(velocity, gravity, time / 2.0);
      updatePosition(position, velocity, gravity, time);
      updateVelocity(velocity, gravity, time / 2.0);

      // Fragments doth rotate
      this->angleShip.add(angularVelocity);

      // Lifetime handling
      framesElapsed++;
      if (framesElapsed >= lifespan)
         kill();

      // Remove if it hits Earth
      if (getAltitude(position) < 0.0)
         kill();
   }
}

/*********************************************
 * FRAGMENT : DRAW
 * Draw the fragment (as a small dot)
 *********************************************/
void Fragment::draw(ogstream& gout)
{
   if (alive)
      gout.drawFragment(position, angleShip.getRadians());
}
