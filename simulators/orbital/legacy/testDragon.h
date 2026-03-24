/***********************************************************************
 * Header File:
 *    TEST DRAGON
 * Author:
 *    Mark Van Horn & Taden Marston
 * Summary:
 *    Unit tests for the Dragon class.
 ************************************************************************/

#pragma once

#include "unitTest.h"
#include "dragon.h"

class TestDragon : public UnitTest
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
      
      report("Dragon");
   }
   
private:
   void constructor_default()
   {
      // Exercise
      DummyDragon dragon;
      
      // Verify initial state
      assertEquals(dragon.isAlive(), true);
      assertEquals(dragon.getPosition().getMetersX(), 0.0);
      assertEquals(dragon.getPosition().getMetersY(), 0.0);
      assertEquals(dragon.getVelocity().getDX(), 0.0);
      assertEquals(dragon.getVelocity().getDY(), 0.0);
   }
   
   void constructor_parameterized()
   {
      // Setup
      MockPosition pos(100.0, 200.0);
      MockVelocity vel(10.0, 20.0);
      
      // Exercise
      DummyDragon dragon(pos, vel);
      
      // Verify initial state
      assertEquals(dragon.isAlive(), true);
      assertEquals(dragon.getPosition().getMetersX(), pos.getMetersX());
      assertEquals(dragon.getPosition().getMetersY(), pos.getMetersY());
      assertEquals(dragon.getVelocity().getDX(), vel.getDX());
      assertEquals(dragon.getVelocity().getDY(), vel.getDY());
   }
   
   void isAlive_defaultTrue()
   {
      // Setup
      DummyDragon dragon;
      
      // Verify
      assertEquals(dragon.isAlive(), true);
   }
   
   void kill_setsAliveToFalse()
   {
      // Setup
      DummyDragon dragon;
      
      // Exercise
      dragon.kill();
      
      // Verify
      assertEquals(!dragon.isAlive(), true);
   }
   
   void position_gettersAndSetters()
   {
      // Setup
      DummyDragon dragon;
      MockPosition newPos(150.0, 250.0);
      
      // Exercise
      dragon.setPosition(newPos);
      
      // Verify
      assertEquals(dragon.getPosition().getMetersX(), newPos.getMetersX());
      assertEquals(dragon.getPosition().getMetersY(), newPos.getMetersY());
   }
   
   void velocity_gettersAndSetters()
   {
      // Setup
      DummyDragon dragon;
      MockVelocity newVel(15.0, 25.0);
      
      // Exercise
      dragon.setVelocity(newVel);
      
      // Verify
      assertEquals(dragon.getVelocity().getDX(), newVel.getDX());
      assertEquals(dragon.getVelocity().getDY(), newVel.getDY());
   }
};
