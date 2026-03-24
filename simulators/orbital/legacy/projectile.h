/***********************************************************************
 * Header File:
 *    PROJECTILE
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Class to represent a projectile in orbit
 ************************************************************************/

#pragma once

#include "OrbitalObject.h"
#include <vector>

class Projectile : public OrbitalObject
{
public:
   // Default Constructor
   Projectile() : OrbitalObject(), framesElapsed(0), lifespan(70)
   {
      radius = ZOOM;
   }

   // Parameterized Constructor
   Projectile(const Position& position,
              const Velocity& velocity,
              int lifespan = 70) : OrbitalObject(position, velocity, Angle()),
                                   framesElapsed(0), lifespan(lifespan)
   {
      radius = ZOOM;
   }

   // Override pure virtual methods from OrbitalObject
   void update(double time) override;
   void draw(ogstream& gout) override;

   // Override onCollision: just die, no new debris
   void onCollision(vector<Part>&, vector<Fragment>&) override { kill(); }

private:
   int framesElapsed;
   int lifespan;
};

#ifdef PROJECTILE_ENABLE_TEST_DOUBLES
class DummyProjectile : public Projectile
{
public:
   DummyProjectile(const Position& p, const Velocity& v, int lifespan = 100) :
   Projectile(p, v, lifespan) { }
   void draw(ogstream&) override { }
};
#endif
