/***********************************************************************
 * Header File:
 *    TEST SPUTNIK
 * Author:
 *    Mark Van Horn & Taden Marston
 * Summary:
 *    Unit tests for the Sputnik class.
 ************************************************************************/

#pragma once

#include "unitTest.h"
#include "sputnik.h"

class TestSputnik : public UnitTest
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
      
      report("Sputnik");
   }
   
private:
   void constructor_default()
   {
      // Exercise
      DummySputnik sputnik;
      
      // Verify initial state
      assertEquals(sputnik.isAlive(), true);
      assertEquals(sputnik.getPosition().getMetersX(), 0.0);
      assertEquals(sputnik.getPosition().getMetersY(), 0.0);
      assertEquals(sputnik.getVelocity().getDX(), 0.0);
      assertEquals(sputnik.getVelocity().getDY(), 0.0);
   }
   
   void constructor_parameterized()
   {
      // Setup
      MockPosition pos(100.0, 200.0);
      MockVelocity vel(10.0, 20.0);
      
      // Exercise
      DummySputnik sputnik(pos, vel);
      
      // Verify initial state
      assertEquals(sputnik.isAlive(), true);
      assertEquals(sputnik.getPosition().getMetersX(), pos.getMetersX());
      assertEquals(sputnik.getPosition().getMetersY(), pos.getMetersY());
      assertEquals(sputnik.getVelocity().getDX(), vel.getDX());
      assertEquals(sputnik.getVelocity().getDY(), vel.getDY());
   }
   
   void isAlive_defaultTrue()
   {
      // Setup
      DummySputnik sputnik;
      
      // Verify
      assertEquals(sputnik.isAlive(), true);
   }
   
   void kill_setsAliveToFalse()
   {
      // Setup
      DummySputnik sputnik;
      
      // Exercise
      sputnik.kill();
      
      // Verify
      assertEquals(!sputnik.isAlive(), true);
   }
   
   void position_gettersAndSetters()
   {
      // Setup
      DummySputnik sputnik;
      MockPosition newPos(150.0, 250.0);
      
      // Exercise
      sputnik.setPosition(newPos);
      
      // Verify
      assertEquals(sputnik.getPosition().getMetersX(), newPos.getMetersX());
      assertEquals(sputnik.getPosition().getMetersY(), newPos.getMetersY());
   }
   
   void velocity_gettersAndSetters()
   {
      // Setup
      DummySputnik sputnik;
      MockVelocity newVel(15.0, 25.0);
      
      // Exercise
      sputnik.setVelocity(newVel);
      
      // Verify
      assertEquals(sputnik.getVelocity().getDX(), newVel.getDX());
      assertEquals(sputnik.getVelocity().getDY(), newVel.getDY());
   }
};
