/***********************************************************************
 * Source File:
 *    SPUTNIK
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Implementation of the Sputnik satellite
 ************************************************************************/

#include "sputnik.h"
#include "uiDraw.h"

void Sputnik::draw(ogstream& gout)
{
   if (isAlive())
      gout.drawSputnik(position, angleShip.getRadians());
}

void Sputnik::onCollision(vector<Part>&, vector<Fragment>& fragments)
{
   // Generate 4 fragments upon collision
   for (int i = 0; i < 4; ++i)
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

      fragments.push_back(Fragment(newPos, newVel));
   }
   kill();
}
