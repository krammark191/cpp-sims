/*************************************************************
 * 1. Name:
 *      Mark Van Horn & Taden Marston
 * 2. Assignment Name:
 *      Lab 11: Orbit Parts and Fragments
 * 3. Assignment Description:
 *      Simulate collisions in space and generate debris
 * 4. What was the hardest part? Be as specific as possible.
 *      The hardest part was getting the collision to work
 *      properly and generate the correct debris.
 * 5. How long did it take for you to complete the assignment?
 *      10 hours
 *****************************************************************/

#include <cassert>         // for ASSERT
#include "uiInteract.h"    // for INTERFACE
#include "uiDraw.h"        // for RANDOM and DRAW*
#include "position.h"      // for POSITION
#include "test.h"          // for TEST
#include "star.h"          // for Star
#include "gps.h"           // for GPS
#include "sputnik.h"       // for Sputnik
#include "hubble.h"        // for Hubble
#include "dragon.h"        // for CrewDragon
#include "starlink.h"      // for Starlink
#include "physics.h"       // for physics
#include <vector>          // for vector
#include "DreamChaser.h"   // for DreamChaser
#include "part.h"          // for Part
#include "fragment.h"      // for Fragment
#include "projectile.h"    // for Projectile

using namespace std;

/*************************************************************************
 * Simulator
 * Structure to capture everything that orbits or interacts
 *************************************************************************/
class Simulator
{
public:
   Simulator(Position ptUpperRight) :
      ptUpperRight(ptUpperRight), angleEarth(0.0),
      GPS1(Position(0.0, 26560000.0),              Velocity(-3880.0, 0.0)),
      GPS2(Position(23001634.72, 13280000.0),      Velocity(-1940.0, 3360.18)),
      GPS3(Position(23001634.72, -13280000.0),     Velocity(1940.0, 3360.18)),
      GPS4(Position(0.0, -26560000.0),             Velocity(3880.0, 0.0)),
      GPS5(Position(-23001634.72, -13280000.0),    Velocity(1940.0, -3360.18)),
      GPS6(Position(-23001634.72, 13280000.0),     Velocity(-1940.0, -3360.18)),
      sputnik(Position(-36515095.13, 21082000.0),  Velocity(2050.0, 2684.68)),
      hubble(Position(0.0, -42164000.0),           Velocity(3100.0, 0.0)),
      dragon(Position(0.0, 8000000.0),             Velocity(-7900.0, 0.0)),
      starlink(Position(0.0, -13020000.0),         Velocity(5800.0, 0.0)),
      dreamChaser()
   {
      if (!stars.empty()) { }
      else
      {
         const int STAR_COUNT = ptUpperRight.getPixelsX();
         for (int i = 0; i < STAR_COUNT; i++)
         {
            Star star;
            star.reset(ptUpperRight.getPixelsX(), ptUpperRight.getPixelsY());
            stars.push_back(star);
         }
      }
      GPS1.setAngleShip(90.0);
      GPS2.setAngleShip(150.0);
      GPS3.setAngleShip(210.0);
      GPS4.setAngleShip(270.0);
      GPS5.setAngleShip(330.0);
      GPS6.setAngleShip(30.0);
   }

   Position ptShip;
   Position ptUpperRight;
   GPS GPS1;
   GPS GPS2;
   GPS GPS3;
   GPS GPS4;
   GPS GPS5;
   GPS GPS6;
   Sputnik sputnik;
   Hubble hubble;
   Dragon dragon;
   Starlink starlink;
   DreamChaser dreamChaser;
   vector<OrbitalObject*> objects
   {
      &GPS1, &GPS2, &GPS3, &GPS4, &GPS5, &GPS6,
      &sputnik, &hubble, &dragon, &starlink, &dreamChaser
   };
   vector<Projectile> projectiles;
   vector<Part> parts;
   vector<Fragment> fragments;
   vector<Star> stars;

   double angleEarth;
};

/*************************************
 * All the interesting work happens here
 **************************************/
void callBack(const Interface* pUI, void* p)
{
   Simulator* pSim = (Simulator*)p;
   pSim->dreamChaser.setThrusting(false);

   // Input
   if (pUI->isLeft())
      pSim->dreamChaser.rotateLeft();
   if (pUI->isRight())
      pSim->dreamChaser.rotateRight();
   if (pUI->isDown())
   {
      pSim->dreamChaser.setThrusting(true);
      pSim->dreamChaser.thrust(getTime());
   }
   if (pUI->isSpace())
      pSim->projectiles.push_back(pSim->dreamChaser.launchProjectile());

   // Update all satellites and DreamChaser
   double t = getTime();
   pSim->GPS1.update(t);
   pSim->GPS2.update(t);
   pSim->GPS3.update(t);
   pSim->GPS4.update(t);
   pSim->GPS5.update(t);
   pSim->GPS6.update(t);
   pSim->sputnik.update(t);
   pSim->hubble.update(t);
   pSim->dragon.update(t);
   pSim->starlink.update(t);
   pSim->dreamChaser.update(t);
   
   // Update projectiles, parts, fragments
   for (auto& projectile : pSim->projectiles)
      projectile.update(t);
   for (auto& part : pSim->parts)
      part.update(t);
   for (auto& fragment : pSim->fragments)
      fragment.update(t);
   
   pSim->angleEarth -= EARTH_SPEED;

   // --- Handle collisions using onCollision ----
   // Gather all alive OrbitalObjects (satellites, DreamChaser, etc.)
   vector<OrbitalObject*> allObjects;
   for (auto* obj : pSim->objects)
      if (obj->isAlive()) allObjects.push_back(obj);
   for (auto& part : pSim->parts)
      if (part.isAlive()) allObjects.push_back(&part);

   // Projectiles vs. everything
   for (auto& proj : pSim->projectiles)
   {
      if (!proj.isAlive()) continue;
      for (auto* obj : allObjects)
      {
         if (obj->isAlive() && detectCollision(proj.getPosition(),
                                               obj->getPosition(),
                                               proj.getRadius(),
                                               obj->getRadius()))
         {
            proj.onCollision(pSim->parts, pSim->fragments);
            obj->onCollision(pSim->parts, pSim->fragments);
         }
      }
   }

   // Fragments vs. everything except other fragments
   for (auto& frag : pSim->fragments)
   {
      if (!frag.isAlive()) continue;
      for (auto* obj : allObjects)
      {
         if (obj->isAlive() && detectCollision(frag.getPosition(),
                                               obj->getPosition(),
                                               frag.getRadius(),
                                               obj->getRadius()))
         {
            frag.onCollision(pSim->parts, pSim->fragments);
            obj->onCollision(pSim->parts, pSim->fragments);
         }
      }
   }

   // Parts vs. parts (avoid double collision)
   for (size_t i = 0; i < pSim->parts.size(); ++i)
   {
      Part& partA = pSim->parts[i];
      if (!partA.isAlive()) continue;
      for (size_t j = i + 1; j < pSim->parts.size(); ++j)
      {
         Part& partB = pSim->parts[j];
         if (!partB.isAlive()) continue;
         if (detectCollision(partA.getPosition(),
                             partB.getPosition(),
                             partA.getRadius(),
                             partB.getRadius()))
         {
            partA.onCollision(pSim->parts, pSim->fragments);
            partB.onCollision(pSim->parts, pSim->fragments);
         }
      }
   }

   // Satellites vs. satellites (avoid double collision)
   for (size_t i = 0; i < pSim->objects.size(); ++i)
   {
      OrbitalObject* objA = pSim->objects[i];
      if (!objA->isAlive()) continue;
      for (size_t j = i + 1; j < pSim->objects.size(); ++j)
      {
         OrbitalObject* objB = pSim->objects[j];
         if (!objB->isAlive()) continue;
         if (detectCollision(objA->getPosition(),
                             objB->getPosition(),
                             objA->getRadius(),
                             objB->getRadius()))
         {
            objA->onCollision(pSim->parts, pSim->fragments);
            objB->onCollision(pSim->parts, pSim->fragments);
         }
      }
   }

   // Remove dead projectiles, parts, and fragments
   pSim->projectiles.erase(remove_if(pSim->projectiles.begin(),
      pSim->projectiles.end(),
      [](const Projectile& p)
      { return !p.isAlive(); }
   ),
      pSim->projectiles.end());
   pSim->parts.erase(remove_if(pSim->parts.begin(),
      pSim->parts.end(),
      [](const Part& p)
      { return !p.isAlive(); }),
      pSim->parts.end());
   pSim->fragments.erase(remove_if(pSim->fragments.begin(),
      pSim->fragments.end(),
      [](const Fragment& f)
      { return !f.isAlive(); }
   ),
      pSim->fragments.end());

   // --- Drawing ---
   Position pt;
   ogstream gout(pt);

   // draw satellites and DreamChaser
   pSim->GPS1.draw(gout);
   pSim->GPS2.draw(gout);
   pSim->GPS3.draw(gout);
   pSim->GPS4.draw(gout);
   pSim->GPS5.draw(gout);
   pSim->GPS6.draw(gout);
   pSim->sputnik.draw(gout);
   pSim->hubble.draw(gout);
   pSim->dragon.draw(gout);
   pSim->starlink.draw(gout);
   pSim->dreamChaser.draw(gout);
   
   // draw projectiles, parts, fragments, and stars
   for (auto& projectile : pSim->projectiles)
      projectile.draw(gout);
   for (auto& part : pSim->parts)
      part.draw(gout);
   for (auto& fragment : pSim->fragments)
      fragment.draw(gout);
   for (Star& star : pSim->stars)
      star.draw(gout);

   pt.setMeters(0.0, 0.0);
   gout.drawEarth(pt, pSim->angleEarth);
}

/*********************************
 * Initialize the simulation and set it in motion
 *********************************/
#ifdef _WIN32_X
#include <windows.h>
int WINAPI wWinMain(
   _In_ HINSTANCE hInstance,
   _In_opt_ HINSTANCE hPrevInstance,
   _In_ PWSTR pCmdLine,
   _In_ int nCmdShow)
#else // !_WIN32
int main(int argc, char** argv)
#endif // !_WIN32
{
   testRunner();

   // Initialize OpenGL
   Position ptUpperRight;
   ptUpperRight.setZoom(128000.0 /* 128km equals 1 pixel */);
   ptUpperRight.setPixelsX(1000.0);
   ptUpperRight.setPixelsY(1000.0);
   Interface ui(0, NULL,
      "Demo",   /* name on the window */
      ptUpperRight);

   // Initialize the simulation
   Simulator sim(ptUpperRight);
//   ui.setFramesPerSecond(10);
   ui.run(callBack, &sim);

   return 0;
}
