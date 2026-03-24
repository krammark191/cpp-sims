/***********************************************************************
 * Header File:
 *    FRAGMENT
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Class to represent a fragment of debris in orbit
 ************************************************************************/

#pragma once

#include "OrbitalObject.h"
#include <vector>

class Fragment : public OrbitalObject
{
   friend class TestFragment;
public:
   // Constructors
   Fragment() : OrbitalObject(), framesElapsed(0), lifespan(random(50, 100))
   {
      radius = 2.0 * ZOOM;
      setAngularVelocity(random(-0.5, 0.5)); // Random angular velocity
   }
   
   Fragment(const Position& position,
            const Velocity& velocity,
            int lifespan = random(50, 100)) :
   OrbitalObject(position, velocity, Angle()),
   framesElapsed(0),
   lifespan(lifespan)
   {
      radius = 2.0 * ZOOM;
      setAngularVelocity(random(-0.5, 0.5)); // Random angular velocity
   }
   
   // Override pure virtual methods from OrbitalObject
   void update(double time) override;
   void draw(ogstream& gout) override;
   
   // Override onCollision: just die, no new fragments/parts
   void onCollision(vector<Part>&, vector<Fragment>&) override { kill(); }
   
private:
   int framesElapsed;
   int lifespan;
};

class DummyFragment : public Fragment
{
public:
   DummyFragment() : Fragment() { }
   DummyFragment(const Position& p, const Velocity& v, int lifespan = 50) :
   Fragment(p, v, lifespan) { }
   void draw(ogstream& /*gout*/) override { }
};
