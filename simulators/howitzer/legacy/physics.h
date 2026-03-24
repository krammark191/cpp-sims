/***********************************************************************
 * Header File:
 *    PHYSICS
 * Author:
 *    Mark Van Horn & Taden Marston
 * Summary:
 *    Laws of motion, effects of gravity, wind resistance, etc.
 ************************************************************************/

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <cassert>  // for ASSERT

/*******************************************************
 * AREA FROM RADIUS
 * Compute the area of a circle from the radius
 *    area = pi * radius ^ 2
 ********************************************************/
inline double areaFromRadius(double radius)
{
   return M_PI * (radius * radius);
}

/**********************************************************
 * FORCE FROM DRAG
 * Determine the drag on a shell based on several things:
 *    density  : The density of the air ?
 *    drag     : The drag coefficient of the shell (no units)
 *    radius   : the radius of the shell (m)
 *    velocity : the velocity of the shell (m/s)
 * This force is determined by
 *    force = 1/2 density drag area velocity ^ 2
 ************************************************************/
inline double forceFromDrag(double density, double drag, double radius, 
	double velocity)
{
	return 0.5 * density * drag * areaFromRadius(radius) * 
		(velocity * velocity);
}

/**********************************************************
 * ACCELERATION FROM FORCE
 * How much acceleration can we expect from a given force
 *    force        : N
 *    mass         : kg
 *    acceleration : m / s^2
 * Equation:
 *    f = m a
 ************************************************************/
inline double accelerationFromForce(double force, double mass)
{
   return force / mass;
}

/***********************************************************
 * VELOCITY FROM ACCELERATION
 * Determine how much velocity will be imparted in a given amount of time
 * from a given acceleration
 *    acceleration : m / s^2
 *    time         : s
 * Equation:
 *    v = a t
 ************************************************************/
inline double velocityFromAcceleration(double acceleration, double time)
{
   return acceleration * time;
}

/*********************************************************
 * LINEAR INTERPOLATION
 * From a list of domains and ranges, linear interpolate
 *
 *    |        *(d1,r1)
 *    |       /
 *    |      *(d,r)
 *   r|     /
 *    |    *(d0,r0)
 *    |
 *    +-------------
 *           d
 * equation:
 *   (r - r0) / (d - d0) = (r1 - r0) / (d1 - d0)
 * Thus:
 *   r  = r0 + (r1 - r0) (d - d0) / (d1 - d0)
 *********************************************************/
inline double linearInterpolation(double d0, double r0,
	double d1, double r1, double d)
{
	return r0 + (r1 - r0) * (d - d0) / (d1 - d0);
}

/*********************************************************
 * MAPPING
 * A simple structure to represent the domain and the range.
 * We use this for linear interpolation
 *********************************************************/
struct Mapping
{
   double domain;
   double range;
};

/*********************************************************
 * LINEAR INTERPOLATION
 * From a list of domains and ranges, linear interpolate
 *********************************************************/
double linearInterpolation(const Mapping mapping[],
                           int numMapping,
                           double domain);

/*********************************************************
 * GRAVITY FROM ALTITUDE
 * Determine gravity coefficient based on the altitude
 *********************************************************/
double gravityFromAltitude(double altitude);

/*********************************************************
 * DENSITY FROM ALTITUDE
 * Determine the density of air based on the altitude
 *********************************************************/
double densityFromAltitude(double altitude);

/*********************************************************
 * SPEED OF SOUND FROM ALTITUDE
 ********************************************************/
double speedSoundFromAltitude(double altitude);

/*********************************************************
 * DRAG FROM MACH
 * Determine the drag coefficient as a function of the speed of sound
 *********************************************************/
double dragFromMach(double speedMach);

