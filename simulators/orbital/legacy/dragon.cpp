/***********************************************************************
 * Source File:
 *    DRAGON
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Implementation of the Crew Dragon spacecraft
 ************************************************************************/

#include "dragon.h"
#include "uiDraw.h"

void Dragon::draw(ogstream& gout)
{
   if (isAlive())
      gout.drawCrewDragon(position, angleShip.getRadians());
}

void Dragon::onCollision(vector<Part>& parts, vector<Fragment>&)
{
   // Dragon: 1 body, 2 panels
   vector<PartType> debris =
   {
      PartType::DRAGON_CENTER,
      PartType::DRAGON_LEFT,
      PartType::DRAGON_RIGHT
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
      newPos.addMetersX(20 * Position::getZoom() * cos(direction));
      newPos.addMetersY(20 * Position::getZoom() * sin(direction));

      parts.push_back(Part(newPos, newVel, pt));
   }
   kill();
}
