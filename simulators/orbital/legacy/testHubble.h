/***********************************************************************
 * Header File:
 *    TEST HUBBLE
 * Author:
 *    Mark Van Horn & Taden Marston
 * Summary:
 *    Unit tests for the Hubble class.
 ************************************************************************/

#pragma once

#include "unitTest.h"
#include "hubble.h"

class TestHubble : public UnitTest
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
      
      report("Hubble");
   }
   
private:
   void constructor_default()
   {
      // Exercise
      DummyHubble hubble;
      
      // Verify initial state
      assertEquals(hubble.isAlive(), true);
      assertEquals(hubble.getPosition().getMetersX(), 0.0);
      assertEquals(hubble.getPosition().getMetersY(), 0.0);
      assertEquals(hubble.getVelocity().getDX(), 0.0);
      assertEquals(hubble.getVelocity().getDY(), 0.0);
   }
   
   void constructor_parameterized()
   {
      // Setup
      MockPosition pos(100.0, 200.0);
      MockVelocity vel(10.0, 20.0);
      
      // Exercise
      DummyHubble hubble(pos, vel);
      
      // Verify initial state
      assertEquals(hubble.isAlive(), true);
      assertEquals(hubble.getPosition().getMetersX(), pos.getMetersX());
      assertEquals(hubble.getPosition().getMetersY(), pos.getMetersY());
      assertEquals(hubble.getVelocity().getDX(), vel.getDX());
      assertEquals(hubble.getVelocity().getDY(), vel.getDY());
   }
   
   void isAlive_defaultTrue()
   {
      // Setup
      DummyHubble hubble;
      
      // Verify
      assertEquals(hubble.isAlive(), true);
   }
   
   void kill_setsAliveToFalse()
   {
      // Setup
      DummyHubble hubble;
      
      // Exercise
      hubble.kill();
      
      // Verify
      assertEquals(!hubble.isAlive(), true);
   }
   
   void position_gettersAndSetters()
   {
      // Setup
      DummyHubble hubble;
      MockPosition newPos(150.0, 250.0);
      
      // Exercise
      hubble.setPosition(newPos);
      
      // Verify
      assertEquals(hubble.getPosition().getMetersX(), newPos.getMetersX());
      assertEquals(hubble.getPosition().getMetersY(), newPos.getMetersY());
   }
   
   void velocity_gettersAndSetters()
   {
      // Setup
      DummyHubble hubble;
      MockVelocity newVel(15.0, 25.0);
      
      // Exercise
      hubble.setVelocity(newVel);
      
      // Verify
      assertEquals(hubble.getVelocity().getDX(), newVel.getDX());
      assertEquals(hubble.getVelocity().getDY(), newVel.getDY());
   }
};
