/***********************************************************************
 * Header File:
 *    VELOCITY
 * Author:
 *    Mark Van Horn & Taden Marston
 * Summary:
 *    Everything we need to know about speed
 ************************************************************************/

#pragma once

// for unit tests
class TestPosition;
class TestVelocity;
class TestAcceleration;
class TestProjectile;

// for add()
class Acceleration;
class Angle;

 /*********************************************
  * Velocity
  * I feel the need, the need for speed
  *********************************************/
class Velocity
{
   // for unit tests
   friend TestPosition;
   friend TestVelocity;
   friend TestProjectile;
   
public:
   // constructors
   Velocity()                     : dx(0.0), dy(0.0) { }
   Velocity(double dx, double dy) : dx(dx), dy(dy)   { }
   Velocity(const Velocity & rhs) : dx(rhs.dx), dy(rhs.dy) { }
   Velocity(double speed, const Angle & angle) : dx(0.0), dy(0.0)
   {
      set(angle, speed);
   }
   Velocity& operator = (const Velocity & rhs)
   {
      if (this != &rhs)
      {
         dx = rhs.dx;
         dy = rhs.dy;
      }
      return *this;
   }

   // getters
   double getDX()       const { return dx; }
   double getDY()       const { return dy; }
   double getSpeed()    const;
   Angle  getAngle()    const;
   
   // setters
   void set(const Angle & angle, double magnitude);
   void setDX(double dx) { this->dx = dx; }
   void setDY(double dy) { this->dy = dy; }
   void setSpeed(double speed);
   void setAngle(const Angle & angle);
   
   void addDX(double dx) { this->dx += dx; }
   void addDY(double dy) { this->dy += dy; }
   void add(const Acceleration & acceleration, double time);
   void add(const Velocity & rhs);
   void reverse();

private:
   double dx;           // horizontal velocity
   double dy;           // vertical velocity
};

/************************************************************
 * VELOCITY : MOCK VELOCITY
 * Mock class for testing Velocity functionality
 * Tracks calls to constructors and setters
 * This class is used to test the Velocity class without
 * requiring the actual Velocity class to be instantiated.
 ************************************************************/
class MockVelocity : public Velocity
{
public:
   // Constructors
   MockVelocity() : Velocity()
   { defaultConstructorCalls++; }
   
   MockVelocity(double dx, double dy) : Velocity(dx, dy)
   { parameterizedConstructorCalls++; }
   
   MockVelocity(const Velocity& rhs) : Velocity(rhs)
   { copyConstructorCalls++; }
   
   MockVelocity(double speed, const Angle& angle) : Velocity(speed, angle)
   { speedAngleConstructorCalls++; }
   
   // Override setters to track calls
   void set(const Angle& angle, double magnitude)
   {
      Velocity::set(angle, magnitude);
      setAngleMagnitudeCalls++;
   }
   
   void setDX(double dx)
   {
      Velocity::setDX(dx);
      setDXCalls++;
   }
   
   void setDY(double dy)
   {
      Velocity::setDY(dy);
      setDYCalls++;
   }
   
   void setSpeed(double speed)
   {
      Velocity::setSpeed(speed);
      setSpeedCalls++;
   }
   
   void setAngle(const Angle& angle)
   {
      Velocity::setAngle(angle);
      setAngleCalls++;
   }
   
   void addDX(double dx)
   {
      Velocity::addDX(dx);
      addDXCalls++;
   }
   
   void addDY(double dy)
   {
      Velocity::addDY(dy);
      addDYCalls++;
   }
   
   void add(const Acceleration& acceleration, double time)
   {
      Velocity::add(acceleration, time);
      addAccelerationCalls++;
   }
   
   void add(const Velocity& rhs)
   {
      Velocity::add(rhs);
      addVelocityCalls++;
   }
   
   void reverse()
   {
      Velocity::reverse();
      reverseCalls++;
   }
   
   // Test helper methods
   static int getDefaultConstructorCalls()      { return defaultConstructorCalls; }
   static int getParameterizedConstructorCalls(){ return parameterizedConstructorCalls; }
   static int getCopyConstructorCalls()         { return copyConstructorCalls; }
   static int getSpeedAngleConstructorCalls()   { return speedAngleConstructorCalls; }
   static int getSetAngleMagnitudeCalls()      { return setAngleMagnitudeCalls; }
   static int getSetDXCalls()                  { return setDXCalls; }
   static int getSetDYCalls()                  { return setDYCalls; }
   static int getSetSpeedCalls()               { return setSpeedCalls; }
   static int getSetAngleCalls()               { return setAngleCalls; }
   static int getAddDXCalls()                  { return addDXCalls; }
   static int getAddDYCalls()                  { return addDYCalls; }
   static int getAddAccelerationCalls()        { return addAccelerationCalls; }
   static int getAddVelocityCalls()            { return addVelocityCalls; }
   static int getReverseCalls()                { return reverseCalls; }
   
   static void reset()
   {
      defaultConstructorCalls = 0;
      parameterizedConstructorCalls = 0;
      copyConstructorCalls = 0;
      speedAngleConstructorCalls = 0;
      setAngleMagnitudeCalls = 0;
      setDXCalls = 0;
      setDYCalls = 0;
      setSpeedCalls = 0;
      setAngleCalls = 0;
      addDXCalls = 0;
      addDYCalls = 0;
      addAccelerationCalls = 0;
      addVelocityCalls = 0;
      reverseCalls = 0;
   }
   
private:
   static int defaultConstructorCalls;
   static int parameterizedConstructorCalls;
   static int copyConstructorCalls;
   static int speedAngleConstructorCalls;
   static int setAngleMagnitudeCalls;
   static int setDXCalls;
   static int setDYCalls;
   static int setSpeedCalls;
   static int setAngleCalls;
   static int addDXCalls;
   static int addDYCalls;
   static int addAccelerationCalls;
   static int addVelocityCalls;
   static int reverseCalls;
};
