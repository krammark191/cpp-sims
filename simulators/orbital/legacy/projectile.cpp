/***********************************************************************
 * Source File:
 *    PROJECTILE
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Implementation of the projectile class
 ************************************************************************/

#include "projectile.h"
#include "uiDraw.h"
#include "physics.h"
#include <cmath>

 /*********************************************
  * PROJECTILE : UPDATE
  * Updates the projectile's position and velocity
  * based on gravity and time, increments lifetime.
  *********************************************/
void Projectile::update(double time)
{
   if (alive)
   {
      // Move projectile
      Acceleration gravity = getGravity(position);
      updateVelocity(velocity, gravity, time / 2.0);
      updatePosition(position, velocity, gravity, time);
      updateVelocity(velocity, gravity, time / 2.0);

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
 * PROJECTILE : DRAW
 * Draw the projectile (as a small dot)
 *********************************************/
void Projectile::draw(ogstream& gout)
{
   if (alive)
      gout.drawProjectile(position);
}
