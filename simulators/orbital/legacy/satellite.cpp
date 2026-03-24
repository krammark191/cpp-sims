/***********************************************************************
 * Source File:
 *    SATELLITE
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Everything related to the satellite class
 ***********************************************************************/
#include "satellite.h"
#include "physics.h" // For getGravity, updateVelocity, updatePosition
#include <cassert>
#include <cmath>

 /*********************************************
  * SATELLITE : UPDATE
  * Updates the satellite's position and velocity
  * based on gravity and time.
  *********************************************/
void Satellite::update(double time)
{
   if (alive)
   {
      // Apply gravity
      Acceleration gravity = getGravity(position);

      // Update velocity and position based on acceleration (gravity)
      updateVelocity(velocity, gravity, time / 2.0);
      updatePosition(position, velocity, gravity, time);
      updateVelocity(velocity, gravity, time / 2.0);

      // Update the angle of the ship
      this->angleShip.add(angularVelocity);

      // Check if the satellite's altitude is too low (crashed into Earth)
      if (getAltitude(position) < 0.0)
         kill(); // Satellite crashes
   }
}

/*********************************************
 * SATELLITE : ONCOLLISION
 * Spawns 2–5 parts, then kills itself.
 *********************************************/
void Satellite::onCollision(vector<Part>& parts, vector<Fragment>&)
{
   int numParts = random(2, 5);
   for (int i = 0; i < numParts; ++i)
   {
      double angle = random(0.0, 2.0 * M_PI);
      double speed = random(1000.0, 6000.0);
      Velocity newVel = getVelocity();
      newVel.addDX(speed * cos(angle));
      newVel.addDY(speed * sin(angle));

      Position newPos = getPosition();
      double direction = atan2(newVel.getDY(), newVel.getDX());
      newPos.addMetersX(4.0 * Position::getZoom() * cos(direction));
      newPos.addMetersY(4.0 * Position::getZoom() * sin(direction));

      parts.push_back(Part(newPos, newVel));
   }
   kill();
}
