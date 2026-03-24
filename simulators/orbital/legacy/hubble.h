/***********************************************************************
 * Header File:
 *    HUBBLE
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Everything related to the Hubble satellite
 ************************************************************************/

#pragma once

#include "satellite.h"
#include "part.h"

class Hubble : public Satellite
{
public:
   Hubble() : Satellite() { radius = 10.0 * ZOOM; }
   Hubble(const Position& position, const Velocity& velocity) :
   Satellite(position, velocity, Angle(90.0))
   {
		radius = 10.0 * ZOOM;
      setAngularVelocity(-EARTH_SPEED + 0.00001175);
	}

   void draw(ogstream& gout) override;
   void onCollision(vector<Part>& parts, vector<Fragment>& fragments) override;
};

class DummyHubble : public Hubble
{
public:
   DummyHubble() : Hubble() { }
   DummyHubble(const Position& p, const Velocity& v) : Hubble(p, v) { }
   void draw(ogstream&) override { }
};
