/***********************************************************************
 * Header File:
 *    STARLINK
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Everything related to the Starlink satellite
 ************************************************************************/

#pragma once

#include "satellite.h"
#include "part.h"

class Starlink : public Satellite
{
public:
   Starlink() : Satellite() { radius = 6.0 * ZOOM; }
   Starlink(const Position& position, const Velocity& velocity) :
   Satellite(position, velocity, Angle()) { radius = 6.0 * ZOOM; }

   void draw(ogstream& gout) override;
   void onCollision(vector<Part>& parts, vector<Fragment>& fragments) override;
};

class DummyStarlink : public Starlink
{
public:
   DummyStarlink() : Starlink() { }
   DummyStarlink(const Position& p, const Velocity& v) : Starlink(p, v) { }
   void draw(ogstream&) override { }
};
