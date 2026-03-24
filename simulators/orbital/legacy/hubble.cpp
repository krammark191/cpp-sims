/***********************************************************************
 * Source File:
 *    HUBBLE
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Implementation of the Hubble satellite
 ************************************************************************/

#include "hubble.h"
#include "uiDraw.h"

void Hubble::draw(ogstream& gout)
{
   if (isAlive())
      gout.drawHubble(position, angleShip.getRadians());
}

void Hubble::onCollision(vector<Part>& parts, vector<Fragment>&)
{
   // Hubble: 1 body, 2 panels, 1 computer
   vector<PartType> debris =
   {
      PartType::HUBBLE_COMPUTER,
      PartType::HUBBLE_TELESCOPE,
      PartType::HUBBLE_RIGHT,
      PartType::HUBBLE_LEFT
   };

   for (auto pt : debris)
   {
      double angle = random(0.0, 2.0 * M_PI);
      double speed = random(1000.0, 6000.0);
      Velocity newVel = this->velocity;
      newVel.addDX(speed * cos(angle));
      newVel.addDY(speed * sin(angle));

      Position newPos = this->position;
      double direction = atan2(newVel.getDY(), newVel.getDX());
      newPos.addMetersX(35.0 * Position::getZoom() * cos(direction));
      newPos.addMetersY(35.0 * Position::getZoom() * sin(direction));

      parts.push_back(Part(newPos, newVel, pt));
   }
   kill();
}
