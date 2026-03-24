/***********************************************************************
 * Source File:
 *    PART
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Implementation for a debris part in orbit
 ************************************************************************/

#include "part.h"
#include "uiDraw.h"

// Update the part's physics (standard orbital update)
void Part::update(double time)
{
   if (!isAlive())
      return;
   
   // Gravity
   Acceleration gravity = getGravity(this->position);
   updateVelocity(velocity, gravity, time / 2.0);
   updatePosition(position, velocity, gravity, time);
   updateVelocity(velocity, gravity, time / 2.0);
   
   // Check if part hits the earth
   if (getAltitude(position) < 0.0)
      kill();
}

// Draw the specific piece of debris
void Part::draw(ogstream& gout)
{
   if (!isAlive())
      return;
   
   switch (type)
   {
         // Hubble parts
      case PartType::HUBBLE_COMPUTER:
         gout.drawHubbleComputer(position, angleShip.getRadians());
         break;
      case PartType::HUBBLE_TELESCOPE:
         gout.drawHubbleTelescope(position, angleShip.getRadians());
         break;
      case PartType::HUBBLE_LEFT:
         gout.drawHubbleLeft(position, angleShip.getRadians());
         break;
      case PartType::HUBBLE_RIGHT:
         gout.drawHubbleRight(position, angleShip.getRadians());
         
         // GPS parts
      case PartType::GPS_CENTER:
         gout.drawGPSCenter(position, angleShip.getRadians());
         break;
      case PartType::GPS_LEFT:
         gout.drawGPSLeft(position, angleShip.getRadians());
         break;
      case PartType::GPS_RIGHT:
         gout.drawGPSRight(position, angleShip.getRadians());
         break;
         
         // Starlink parts
      case PartType::STARLINK_BODY:
         gout.drawStarlinkBody(position, angleShip.getRadians());
         break;
      case PartType::STARLINK_ARRAY:
         gout.drawStarlinkArray(position, angleShip.getRadians());
         break;
         
         // Dragon parts
      case PartType::DRAGON_CENTER:
         gout.drawCrewDragonCenter(position, angleShip.getRadians());
         break;
      case PartType::DRAGON_LEFT:
         gout.drawCrewDragonLeft(position, angleShip.getRadians());
         break;
      case PartType::DRAGON_RIGHT:
         gout.drawCrewDragonRight(position, angleShip.getRadians());
         break;
         
         // Fallback for anything else
      case PartType::GENERIC:
      default:
         gout.drawFragment(position, angleShip.getRadians());
         break;
   }
}

// When a part is destroyed, spawn fragments
void Part::onCollision(vector<Part>&, vector<Fragment>& fragments)
{
   int numFragments = 4; // Default
   
   // Set number of fragments based on part type
   switch (type)
   {
      case PartType::HUBBLE_TELESCOPE:
         numFragments = 3;
         break;
      case PartType::HUBBLE_COMPUTER:
      case PartType::HUBBLE_LEFT:
      case PartType::HUBBLE_RIGHT:
         numFragments = 2;
         break;
         
      case PartType::GPS_CENTER:
      case PartType::GPS_LEFT:
      case PartType::GPS_RIGHT:
         numFragments = 3;
         break;
         
      case PartType::DRAGON_CENTER:
         numFragments = 4;
         break;
      case PartType::DRAGON_LEFT:
      case PartType::DRAGON_RIGHT:
         numFragments = 2;
         break;
      case PartType::STARLINK_BODY:
      case PartType::STARLINK_ARRAY:
         numFragments = 3;
         break;
         // Add more cases as needed for other types
      default:
         numFragments = 3;
         break;
   }
   
   for (int i = 0; i < numFragments; ++i)
   {
      double angle = random(0.0, 2.0 * M_PI);
      double speed = random(500.0, 2000.0);
      Velocity fragVel = this->velocity;
      fragVel.addDX(speed * cos(angle));
      fragVel.addDY(speed * sin(angle));
      
      Position fragPos = this->position;
      double direction = atan2(fragVel.getDY(), fragVel.getDX());
      fragPos.addMetersX(2.0 * Position::getZoom() * cos(direction));
      fragPos.addMetersY(2.0 * Position::getZoom() * sin(direction));
      
      fragments.push_back(Fragment(fragPos, fragVel));
   }
   kill();
}
