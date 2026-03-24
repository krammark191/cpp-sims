/***********************************************************************
 * Source File:
 *    GPS
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Implementation of the GPS satellite
 ************************************************************************/

#include "gps.h"
#include "uiDraw.h"

void GPS::draw(ogstream& gout)
{
   if (isAlive())
      gout.drawGPS(position, angleShip.getRadians());
}

void GPS::onCollision(vector<Part>& parts, vector<Fragment>& fragments)
{
   // GPS: 1 center, 2 solar panels
   vector<PartType> debris =
   {
      PartType::GPS_CENTER,
      PartType::GPS_LEFT,
      PartType::GPS_RIGHT
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
      newPos.addMetersX(40.0 * Position::getZoom() * cos(direction));
      newPos.addMetersY(40.0 * Position::getZoom() * sin(direction));

      parts.push_back(Part(newPos, newVel, pt));
   }

   // Generate 2 fragments
   for (int i = 0; i < 2; ++i)
   {
      double angle = random(0.0, 2.0 * M_PI);
      double speed = random(1000.0, 6000.0);
      Velocity fragVel = this->velocity;
      fragVel.addDX(speed * cos(angle));
      fragVel.addDY(speed * sin(angle));

      Position fragPos = this->position;
      double direction = atan2(fragVel.getDY(), fragVel.getDX());
      fragPos.addMetersX(25.0 * Position::getZoom() * cos(direction));
      fragPos.addMetersY(25.0 * Position::getZoom() * sin(direction));

      fragments.push_back(Fragment(fragPos, fragVel));
   }

   kill();
}
