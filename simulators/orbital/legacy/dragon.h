/***********************************************************************
 * Header File:
 *    DRAGON
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Everything related to the Crew Dragon spacecraft
 ************************************************************************/

#pragma once

#include "satellite.h"
#include "part.h"

class Dragon : public Satellite
{
public:
   Dragon() : Satellite() { radius = 7.0 * ZOOM; }
   Dragon(const Position& position, const Velocity& velocity) :
   Satellite(position, velocity, Angle()) { radius = 7.0 * ZOOM; }

   void draw(ogstream& gout) override;
   void onCollision(vector<Part>& parts, vector<Fragment>& fragments) override;
};

class DummyDragon : public Dragon
{
public:
   DummyDragon() : Dragon() { }
   DummyDragon(const Position& p, const Velocity& v) : Dragon(p, v) { }
   void draw(ogstream&) override { }
};
