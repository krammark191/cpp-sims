/***********************************************************************
 * Header File:
 *    TEST STARLINK
 * Author:
 *    Mark Van Horn & Taden Marston
 * Summary:
 *    Unit tests for the Starlink class.
 ************************************************************************/

#pragma once

#include "unitTest.h"
#include "starlink.h"

class TestStarlink : public UnitTest
{
public:
   void run()
   {
      constructor_default();
      constructor_parameterized();
      isAlive_defaultTrue();
      kill_setsAliveToFalse();
      position_gettersAndSetters();
      velocity_gettersAndSetters();
      
      report("Starlink");
   }
   
private:
   void constructor_default()
   {
      // Exercise
      DummyStarlink starlink;
      
      // Verify initial state
      assertEquals(starlink.isAlive(), true);
      assertEquals(starlink.getPosition().getMetersX(), 0.0);
      assertEquals(starlink.getPosition().getMetersY(), 0.0);
      assertEquals(starlink.getVelocity().getDX(), 0.0);
      assertEquals(starlink.getVelocity().getDY(), 0.0);
   }
   
   void constructor_parameterized()
   {
      // Setup
      MockPosition pos(100.0, 200.0);
      MockVelocity vel(10.0, 20.0);
      
      // Exercise
      DummyStarlink starlink(pos, vel);
      // Verify initial state
      assertEquals(starlink.isAlive(), true);
      assertEquals(starlink.getPosition().getMetersX(), pos.getMetersX());
      assertEquals(starlink.getPosition().getMetersY(), pos.getMetersY());
      assertEquals(starlink.getVelocity().getDX(), vel.getDX());
      assertEquals(starlink.getVelocity().getDY(), vel.getDY());
   }
   
   void isAlive_defaultTrue()
   {
      // Setup
      DummyStarlink starlink;
      
      // Verify
      assertEquals(starlink.isAlive(), true);
   }
   
   void kill_setsAliveToFalse()
   {
      // Setup
      DummyStarlink starlink;
      
      // Exercise
      starlink.kill();
      
      // Verify
      assertEquals(!starlink.isAlive(), true);
   }
   
   void position_gettersAndSetters()
   {
      // Setup
      DummyStarlink starlink;
      MockPosition newPos(150.0, 250.0);
      
      // Exercise
      starlink.setPosition(newPos);
      
      // Verify
      assertEquals(starlink.getPosition().getMetersX(), newPos.getMetersX());
      assertEquals(starlink.getPosition().getMetersY(), newPos.getMetersY());
   }
   
   void velocity_gettersAndSetters()
   {
      // Setup
      DummyStarlink starlink;
      MockVelocity newVel(15.0, 25.0);
      
      // Exercise
      starlink.setVelocity(newVel);
      
      // Verify
      assertEquals(starlink.getVelocity().getDX(), newVel.getDX());
      assertEquals(starlink.getVelocity().getDY(), newVel.getDY());
   }
};
