/***********************************************************************
 * Header File:
 *    ANGLE
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Everything we need to know about a direction
 ************************************************************************/

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>   // for M_PI which is 3.14159

 // for the unit tests
class TestAngle;
class TestPosition;
class TestVelocity;
class TestAcceleration;

 /************************************
  * ANGLE
  ************************************/
class Angle
{
public:
   // for the unit tests
   friend TestAcceleration;
   friend TestVelocity;
   friend TestAngle;

   // Constructors
   Angle() : radians(0.0) {}
   Angle(const Angle& rhs) : radians(rhs.radians) {}
   Angle(double degrees) : radians(convertToRadians(degrees)) {}

   // Getters
   double getDegrees() const { return convertToDegrees(radians); }
   double getRadians() const { return radians; }

   //         dx
   //    +-------/
   //    |      /
   // dy |     /
   //    |    / 1.0
   //    | a /
   //    |  /
   //    | /
   // dy = cos a
   // dx = sin a
   double getDx()    const { return sin(radians); }
   double getDy()    const { return cos(radians); }
   bool   isRight()  const { return (radians > 0 && radians < M_PI); }
   bool   isLeft()   const { return (radians > M_PI && radians < M_PI * M_PI); }


   // Setters
   void setDegrees(double degrees) { radians = convertToRadians(degrees); }
   void setRadians(double radians) { this->radians = normalize(radians); }
   void setUp()                    { radians = 0.0; }
   void setDown()                  { radians = M_PI; }
   void setRight()                 { radians = M_PI_2; }
   void setLeft()                  { radians = M_PI + M_PI_2; }
   void reverse()                  { radians += M_PI; }
   Angle& add(double delta) { radians += normalize(delta); return *this; }

   // set based on the components
   //         dx
   //     +-------/
   //     |      /
   //  dy |     /
   //     |    /
   //     | a /
   //     |  /
   //     | /
   void setDxDy(double dx, double dy) { radians = normalize(atan2(dx, dy)); }
   Angle operator+(double degrees) const { return Angle(); }

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

#include <iostream>

/*******************************************************
 * OUTPUT ANGLE
 * place output on the screen in degrees
 *******************************************************/
inline std::ostream& operator << (std::ostream& out, const Angle& rhs)
{
   out << rhs.getDegrees() << "degree";
   return out;
}

/************************************************************
 * ANGLE : MOCK ANGLE
 * Mock class for testing Angle functionality
 * Tracks calls to constructors and setters
 * This class is used to test the Angle class without
 * requiring the actual Angle class to be instantiated.
 ************************************************************/
class MockAngle : public Angle
{
public:
   // Constructors
   MockAngle() : Angle()
   { defaultConstructorCalls++; }
   
   MockAngle(const Angle& rhs) : Angle(rhs)
   { copyConstructorCalls++; }
   
   MockAngle(double degrees) : Angle(degrees)
   { degreeConstructorCalls++; }
   
   // Override setters to track calls
   void setDegrees(double degrees)
   {
      Angle::setDegrees(degrees);
      setDegreesCalls++;
   }
   
   void setRadians(double radians)
   {
      Angle::setRadians(radians);
      setRadiansCalls++;
   }
   
   void setDxDy(double dx, double dy)
   {
      Angle::setDxDy(dx, dy);
      setDxDyCalls++;
   }
   
   // Test helper methods
   static int getDefaultConstructorCalls()  { return defaultConstructorCalls; }
   static int getCopyConstructorCalls()     { return copyConstructorCalls; }
   static int getDegreeConstructorCalls()   { return degreeConstructorCalls; }
   static int getSetDegreesCalls()         { return setDegreesCalls; }
   static int getSetRadiansCalls()         { return setRadiansCalls; }
   static int getSetDxDyCalls()           { return setDxDyCalls; }
   
   static void reset()
   {
      defaultConstructorCalls = 0;
      copyConstructorCalls = 0;
      degreeConstructorCalls = 0;
      setDegreesCalls = 0;
      setRadiansCalls = 0;
      setDxDyCalls = 0;
   }
   
private:
   static int defaultConstructorCalls;
   static int copyConstructorCalls;
   static int degreeConstructorCalls;
   static int setDegreesCalls;
   static int setRadiansCalls;
   static int setDxDyCalls;
};
