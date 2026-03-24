/***********************************************************************
 * Header File:
 *    ANGLE
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Everything we need to know about a direction
 ************************************************************************/

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>   // for M_PI which is 3.14159
#include <cmath> // for nice little abs() functions

class TestPosition;
class TestVelocity;
class TestAcceleration;
class TestAngle;
class TestLander;

 /************************************
  * ANGLE
  ************************************/
class Angle
{
public:
   friend TestAcceleration;
   friend TestVelocity;
   friend TestAngle;
   friend TestLander;
   
   // Constructors
   Angle()                 : radians(0.0)  {}
   Angle(const Angle& rhs) : radians(rhs.radians)  {}
   Angle(double degrees)   : radians(convertToRadians(degrees))  {}

   // Getters
   double getDegrees() const { return convertToDegrees(radians); }
   double getRadians() const { return radians; }

   // Setters
   void setDegrees(double degrees) { radians = convertToRadians(degrees); }
   void setRadians(double radian) { radians = normalize(radian); }
   void setUp()                    { radians = convertToRadians(0); }
   void setDown()                  { radians = convertToRadians(180); }
   void setRight()                 { radians = convertToRadians(90); }
   void setLeft()                  { radians = convertToRadians(270); }
   void reverse()                  { radians =  normalize(radians - M_PI); }
   Angle& add(double delta)
   {
      radians += delta;
      radians = normalize(radians);
      return *this;
   }

private:
   double normalize(double radians) const;

   double convertToDegrees(double radian) const
   {
      return normalize(radian) * (180 / M_PI);
   }

   double convertToRadians(double degrees) const
   {
      return normalize(degrees * (M_PI / 180));
   }

   double radians;   // 360 degrees equals 2 PI radians
};
