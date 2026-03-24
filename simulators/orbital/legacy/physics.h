/***********************************************************************
 * Header File:
 *    PHYSICS
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Everything we need to know about fezziks
 ************************************************************************/

#pragma once

#include <cmath>
#include "position.h"
#include "velocity.h"
#include "acceleration.h"
#include "angle.h"
#include <vector>

using namespace std;

const double EARTH_RADIUS = 6378000.0;

double calculateEarthSpeed();
extern const double EARTH_SPEED;

double getTime();

double getAltitude(const Position& pos);
Acceleration getGravity(const Position& pos);
Velocity& updateVelocity(Velocity& velocity,
   const Acceleration& acceleration,
   double time);
Position& updatePosition(Position& position,
   const Velocity& velocity,
   const Acceleration& acceleration,
   double time);

bool detectCollision(const Position& pos1, const Position& pos2,
                     double radius1, double radius2);

