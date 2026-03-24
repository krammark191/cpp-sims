/***********************************************************************
 * Header File:
 *    SPUTNIK
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Everything related to the Sputnik satellite
 ************************************************************************/

#pragma once

#include "satellite.h"
#include "part.h"

class Sputnik : public Satellite
{
public:
   Sputnik() : Satellite() { radius = 4.0 * ZOOM; }

   Sputnik(const Position& position, const Velocity& velocity) :
   Satellite(position, velocity, Angle())
   {
      radius = 4.0 * ZOOM;
      setAngularVelocity(random(-EARTH_SPEED, EARTH_SPEED) * 4);
   }

   void draw(ogstream& gout) override;
   void onCollision(vector<Part>& parts, vector<Fragment>& fragments) override;
};

class DummySputnik : public Sputnik
{
public:
   DummySputnik() : Sputnik() { }
   DummySputnik(const Position& p, const Velocity& v) : Sputnik(p, v) { }
   void draw(ogstream&) override { }
};

