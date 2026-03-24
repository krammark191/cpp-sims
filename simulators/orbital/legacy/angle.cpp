/***********************************************************************
 * Source File:
 *    ANGLE
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Everything we need to know about a direction
 ************************************************************************/

#include "angle.h"
#include <cmath>   // for fmod()
#include <cassert>
using namespace std;

/************************************
 * ANGLE : NORMALIZE
 ************************************/
double Angle::normalize(double radians) const
{
	radians = fmod(radians, 2.0 * M_PI);
	if (radians < 0)
		radians += 2.0 * M_PI;
	return radians;

}

int MockAngle::defaultConstructorCalls = 0;
int MockAngle::copyConstructorCalls = 0;
int MockAngle::degreeConstructorCalls = 0;
int MockAngle::setDegreesCalls = 0;
int MockAngle::setRadiansCalls = 0;
int MockAngle::setDxDyCalls = 0;
