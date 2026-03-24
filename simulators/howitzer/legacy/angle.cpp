/***********************************************************************
 * Source File:
 *    ANGLE
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Everything we need to know about a direction
 ************************************************************************/

#include "angle.h"
#include <math.h>  // for floor()
#include <cassert>
using namespace std;

/************************************
 * ANGLE : NORMALIZE
 ************************************/
double Angle::normalize(double radians) const
{
   radians = fmod(radians, 2.0 * M_PI);
   if (radians < 0.0)
      radians += 2.0 * M_PI;
   return radians;
}



