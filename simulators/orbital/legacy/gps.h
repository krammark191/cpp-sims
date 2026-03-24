/***********************************************************************
 * Header File:
 *    GPS
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Everything related to the GPS satellite
 ************************************************************************/

#pragma once

#include "satellite.h"
#include "part.h"

class GPS : public Satellite
{
public:
   GPS() : Satellite() { radius = 12.0 * ZOOM; }
   GPS(const Position& position, const Velocity& velocity) :
   Satellite(position, velocity, Angle())
   {
      radius = 12.0 * ZOOM;
      setAngularVelocity(-EARTH_SPEED * 2.0 + 0.0000235);
   }
   
   void draw(ogstream& gout) override;
   void onCollision(vector<Part>& parts, vector<Fragment>& fragments) override;
};

class DummyGPS : public GPS
{
public:
   DummyGPS() : GPS() { }
   DummyGPS(const Position& p, const Velocity& v) : GPS(p, v) { }
   void draw(ogstream&) override { }
};
