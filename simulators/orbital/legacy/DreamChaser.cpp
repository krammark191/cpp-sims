/***********************************************************************
 * Source File:
 *    DREAM CHASER
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Everything related to the Dream Chaser Spacecraft.
 ***********************************************************************/

#include "DreamChaser.h"
#include <cassert> // For assertions>

 /*********************************************
  * DREAM CHASER :: CONSTRUCTOR
  * Initializes the Dream Chaser with a default position
  * and velocity.
  *********************************************/
DreamChaser::DreamChaser() : OrbitalObject()
{
   Position initialPos;
   initialPos.setPixelsX(-450.0);
   initialPos.setPixelsY(450.0);
   this->position = initialPos;

   this->velocity = Velocity(0.0, -2000.0);

   this->angleShip = Angle(0.0);
   radius = 10.0 * ZOOM;
}

/*********************************************
 * DREAM CHASER :: CONSTRUCTOR (Parameterized)
 *********************************************/
DreamChaser::DreamChaser(const Position& position, const Velocity& velocity)
   : OrbitalObject(position, velocity, Angle(0.0))
{
   radius = 10.0 * ZOOM;
}

/*********************************************
 * DREAM CHASER :: ROTATE LEFT
 *********************************************/
void DreamChaser::rotateLeft()
{
   this->angleShip.add(-ROTATION_AMOUNT);
}

/*********************************************
 * DREAM CHASER :: ROTATE RIGHT
 *********************************************/
void DreamChaser::rotateRight()
{
   this->angleShip.add(ROTATION_AMOUNT);
}

/*********************************************
 * DREAM CHASER :: THRUST
 *********************************************/
void DreamChaser::thrust(double time)
{
   Acceleration thrustAccel(THRUST_MAGNITUDE, this->angleShip);
   updateVelocity(this->velocity, thrustAccel, time);
}

/*********************************************
 * DREAM CHASER :: LAUNCH PROJECTILE
 *********************************************/
Projectile DreamChaser::launchProjectile()
{
   Position projectileStartPos = this->position;
   Position temp;
   double offsetMeters = PROJECTILE_OFFSET * temp.getZoom();
   double dxOffset = sin(this->angleShip.getRadians()) * offsetMeters;
   double dyOffset = cos(this->angleShip.getRadians()) * offsetMeters;

   projectileStartPos.addMetersX(dxOffset);
   projectileStartPos.addMetersY(dyOffset);

   Velocity projectileVelocity = this->velocity;
   double boostDx = sin(this->angleShip.getRadians()) * PROJECTILE_SPEED_BOOST;
   double boostDy = cos(this->angleShip.getRadians()) * PROJECTILE_SPEED_BOOST;

   projectileVelocity.addDX(boostDx);
   projectileVelocity.addDY(boostDy);

   return Projectile(projectileStartPos, projectileVelocity, 70);
}

/*********************************************
 * DREAM CHASER :: UPDATE
 *********************************************/
void DreamChaser::update(double time)
{
   if (!isAlive())
      return;

   Acceleration gravity = getGravity(this->position);
   updateVelocity(this->velocity, gravity, time);
   updatePosition(this->position, this->velocity, gravity, time);
}

/*********************************************
 * DREAM CHASER :: DRAW
 *********************************************/
void DreamChaser::draw(ogstream& gout)
{
   if (isAlive())
      gout.drawShip(position, angleShip.getRadians(), isThrusting);
}

/*********************************************
 * DREAM CHASER :: ONCOLLISION
 * Spawns 2–5 parts, then kills itself.
 *********************************************/
void DreamChaser::onCollision(vector<Part>& parts, vector<Fragment>&)
{
   int numParts = random(2, 5);
   for (int i = 0; i < numParts; ++i)
   {
      double angle = random(0.0, 2.0 * M_PI);
      double speed = random(1000.0, 6000.0);
      Velocity newVel = this->velocity;
      newVel.addDX(speed * cos(angle));
      newVel.addDY(speed * sin(angle));

      Position newPos = this->position;
      double direction = atan2(newVel.getDY(), newVel.getDX());
      newPos.addMetersX(4.0 * Position::getZoom() * cos(direction));
      newPos.addMetersY(4.0 * Position::getZoom() * sin(direction));

      parts.push_back(Part(newPos, newVel));
   }
   kill();
}
