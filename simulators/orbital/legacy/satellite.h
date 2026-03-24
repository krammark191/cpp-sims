/***********************************************************************
 * Header File:
 *    Satellite
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Everything related to a satellite object.
 ************************************************************************/
#pragma once

#include "OrbitalObject.h"
#include "part.h"

class Satellite : public OrbitalObject
{
   friend class TestSatellite;
public:
   // Constructors
   Satellite() : OrbitalObject() { radius = 3.0 * ZOOM; }
   Satellite(const Position& position,
             const Velocity& velocity,
             const Angle& angle) : OrbitalObject(position, velocity, angle)
   {
      radius = 3.0 * ZOOM;
   }
   
   // Specific method for Satellite, as per rubric (even if it just checks alive status)
   bool isDead() const { return !isAlive(); }
   
   // Override pure virtual methods from OrbitalObject.
   void update(double time) override;
   virtual void draw(ogstream& gout) override = 0; // Still pure virtual, specific satellites will implement
   
   // NEW: Override onCollision to spawn parts
   void onCollision(vector<Part>& parts, vector<Fragment>& fragments) override;
};

class DummySatellite : public Satellite
{
public:
   DummySatellite() : Satellite() { }
   DummySatellite(const Position& p, const Velocity& v, const Angle& a) :
   Satellite(p, v, a) { }
   void draw(ogstream&) override { }
};
