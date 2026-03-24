/***********************************************************************
 * Source File:
 *    STARLINK
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Implementation of the Starlink satellite
 ************************************************************************/

#include "starlink.h"
#include "uiDraw.h"

void Starlink::draw(ogstream& gout)
{
   if (isAlive())
      gout.drawStarlink(position, angleShip.getRadians());
}

void Starlink::onCollision(vector<Part>& parts, vector<Fragment>&)
{
   // Starlink: 1 body, 1 array
   vector<PartType> debris =
   {
      PartType::STARLINK_BODY,
      PartType::STARLINK_ARRAY,
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
      newPos.addMetersX(15.0 * Position::getZoom() * cos(direction));
      newPos.addMetersY(15.0 * Position::getZoom() * sin(direction));

      parts.push_back(Part(newPos, newVel, pt));
   }
   kill();
}
