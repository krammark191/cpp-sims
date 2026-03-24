/**********************************************************************
 * Source File:
 *    SIMULATION
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Execute one simulation of a projectile being fired.
 ************************************************************************/

#include "simulation.h"  // for SIMULATION
#include <iomanip>       // for std::setw and std::setfill

Simulator::Simulator(const Position& posUpperRight)
: ground(posUpperRight), howitzer(), posUpperRight(posUpperRight)
{
   // initializers
   time = 0.0;
   angle = 0.0;
   isFiring = false;
   isHit = false;
   
   // Set horizontal position of the howitzer
   posHowitzer.setPixelsX(Position(posUpperRight).getPixelsX() / 2.0);
   
   // Draw the ground and set the vertical position of the howitzer
   ground.reset(posHowitzer);
   
   // Use the howitzer position object to reduce redundancy
   howitzer.setPosition(posHowitzer);
}

void Simulator::update(double timeStep)
{
   if (isFiring)
   {
      time += timeStep;
      projectile.advance(time);
      if (projectile.getPosition().getMetersY()
          < ground.getElevationMeters(projectile.getPosition()))
      {
         isFiring = false;
         if (abs(projectile.getPosition().getMetersX()
                 - ground.getTarget().getMetersX()) < 175.0)
         {
            isHit = true;
            ground.reset(howitzer.getPosition());
         }
         else
         {
            isHit = false;
         }
         projectile.reset();
      }
   }

   // Shift old positions in the projectilePath array so the trail is preserved
   for (int i = 19; i > 0; i--)
   {
      projectilePath[i] = projectilePath[i - 1];
   }
   
   // Store the current projectile position at the beginning of the trail
   projectilePath[0] = projectile.getPosition();
}

void Simulator::handleInput(const Interface* pUI)
{
   if (pUI->isRight())
      howitzer.rotate(0.05);
   if (pUI->isLeft())
      howitzer.rotate(-0.05);
   if (pUI->isUp())
      howitzer.raise(0.003);
   if (pUI->isDown())
      howitzer.raise(-0.003);
   if (pUI->isSpace() && !isFiring)
   {
      time = 0.0;
      projectile.fire(howitzer.getPosition(), howitzer.getElevation(),
                      howitzer.getMuzzleVelocity(), time);
      isFiring = true;
      isHit = false;
   }
}

void Simulator::draw(ogstream& gout) const
{
   ground.draw(gout);
   howitzer.draw(gout, time);
   for (int i = 0; i < 20; ++i)
   {
      gout.drawProjectile(projectilePath[i], i * 0.5);
   }

   // Draw some status text on the screen
   gout.setf(std::ios::fixed | std::ios::showpoint);
   gout.precision(1);
   gout << std::setw(100) << std::setfill(' ') << ""
        << "Time since the bullet was fired: " << time << "s\n";
   gout << std::setw(111) << std::setfill(' ') << ""
        << "Howitzer angle: " << howitzer.getElevation().getDegrees()
        << " degrees\n";
   if (!isFiring)
   {
      if (isHit)
      {
         gout << std::setw(111) << std::setfill(' ') << "" << "Target: Hit!";
      }
      else
      {
         gout << std::setw(111) << std::setfill(' ') << "" << "Target: Miss!";
      }
   }
   else
   {
      gout << std::setw(111) << std::setfill(' ') << "" << "";
   }
}

Position Simulator::getPosUpperRight() const
{
   return posUpperRight;
}

