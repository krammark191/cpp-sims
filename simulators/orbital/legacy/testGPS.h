/***********************************************************************
 * Header File:
 *    TEST GPS
 * Author:
 *    Mark Van Horn & Taden Marston
 * Summary:
 *    Unit tests for the GPS class.
 ************************************************************************/

#pragma once

#include "unitTest.h"
#include "gps.h"
#include "position.h"
#include "velocity.h"

class TestGPS : public UnitTest
{
public:
   void run()
   {
      defaultConstructor_setsDefaultValues();
      parameterizedConstructor_setsValues();
      getPosition_returnsCorrectValue();
      getVelocity_returnsCorrectValue();
      
      report("GPS");
   }
   
private:
   void defaultConstructor_setsDefaultValues()
   {
      // Setup
      MockPosition expectedPos;  // (0,0)
      MockVelocity expectedVel;  // (0,0)
      
      // Exercise
      DummyGPS gps;
      
      // Verify
      assertEquals(gps.getPosition().getMetersX(), expectedPos.getMetersX());
      assertEquals(gps.getPosition().getMetersY(), expectedPos.getMetersY());
      assertEquals(gps.getVelocity().getDX(), expectedVel.getDX());
      assertEquals(gps.getVelocity().getDY(), expectedVel.getDY());
   }
   
   void parameterizedConstructor_setsValues()
   {
      // Setup
      MockPosition pos(100.0, 200.0);
      MockVelocity vel(10.0, 20.0);
      
      // Exercise
      DummyGPS gps(pos, vel);
      
      // Verify
      assertEquals(gps.getPosition().getMetersX(), pos.getMetersX());
      assertEquals(gps.getPosition().getMetersY(), pos.getMetersY());
      assertEquals(gps.getVelocity().getDX(), vel.getDX());
      assertEquals(gps.getVelocity().getDY(), vel.getDY());
   }
   
   void getPosition_returnsCorrectValue()
   {
      // Setup
      MockPosition pos(100.0, 200.0);
      MockVelocity vel(10.0, 20.0);
      DummyGPS gps(pos, vel);
      
      // Exercise
      MockPosition result = gps.getPosition();
      
      // Verify
      assertEquals(result.getMetersX(), pos.getMetersX());
      assertEquals(result.getMetersY(), pos.getMetersY());
   }
   
   void getVelocity_returnsCorrectValue()
   {
      // Setup
      MockPosition pos(100.0, 200.0);
      MockVelocity vel(10.0, 20.0);
      DummyGPS gps(pos, vel);
      
      // Exercise
      MockVelocity result = gps.getVelocity();
      
      // Verify
      assertEquals(result.getDX(), vel.getDX());
      assertEquals(result.getDY(), vel.getDY());
   }
};
