/***********************************************************************
 * Header File:
 *    ACCELERATION 
 * Author:
 *    Mark Van Horn & Taden Marston
 * Summary:
 *    Everything we need to know about acceleration
 ************************************************************************/


#pragma once

class TestAcceleration;
class TestVelocity;
class TestPosition;
class Angle;


/*********************************************
 * Acceleration
 * Let's get moving
 *********************************************/
class Acceleration
{
   friend TestPosition;
   friend TestVelocity;
   friend TestAcceleration;

public:
   // constructors
   Acceleration()                       : ddx(0.0), ddy(0.0) { }
   Acceleration(double ddx, double ddy) : ddx(), ddy()
   {
      setDDX(ddx);
      setDDY(ddy);
   }
   Acceleration(double acc, const Angle& angle) : ddx(0.0), ddy(0.0)
   {
      set(angle, acc);
   }

   // getters
   double getDDX()   const           { return ddx; }
   double getDDY()   const           { return ddy; }

   // setters
   virtual void setDDX(double ddx)           { this->ddx = ddx; }
   virtual void setDDY(double ddy)           { this->ddy = ddy; }
   void set(const Angle & a, double magnitude);
   void addDDX(double ddx)                   { this->ddx += ddx; }
   void addDDY(double ddy)                   { this->ddy += ddy; }
   void add(const Acceleration& rhs);

private:
   double ddx;     // horizontal acceleration
   double ddy;     // vertical acceleration
};


/*********************************************
 * Dummy Acceleration
 * A test double for the Acceleration class
 *********************************************/
class DummyAcceleration : public Acceleration
{

public:
   int numConstructorCalls;
   int numParameterizedConstructorCalls;
   int numSetDDXCalls;
   int numSetDDYCalls;
   
   // constructors
   DummyAcceleration() : Acceleration()
   {
      numConstructorCalls++;
   }
   
   DummyAcceleration(double ddx, double ddy) : Acceleration(ddx, ddy)
   {
      numParameterizedConstructorCalls++;
   }
   
   // mock-specific methods to verify behavior
   void reset()
   {
      numConstructorCalls = 0;
      numParameterizedConstructorCalls = 0;
      numSetDDXCalls = 0;
      numSetDDYCalls = 0;
   }
   
   void setDDX(double ddx) override
   {
      numSetDDXCalls++;
      Acceleration::setDDX(ddx);
   }
   
   void setDDY(double ddy) override
   {
      numSetDDYCalls++;
      Acceleration::setDDY(ddy);
   }
};
