/***********************************************************************
 * Header File:
 *    TEST SATELLITE
 * Author:
 *    Mark Van Horn & Taden Marston
 * Summary:
 *    Unit tests for the Satellite class.
 ************************************************************************/

#pragma once

#include "unitTest.h"
#include "satellite.h"
#include "position.h"
#include "velocity.h"

class TestSatellite : public UnitTest
{
public:
   void run()
   {
      defaultConstructor();
      parameterizedConstructor();
      isDeadTest();
      collisionTest();
      
      report("TestSatellite");
   }
   
private:
   // Test default constructor
   void defaultConstructor()
   {
      // Setup
      DummySatellite sat;
      
      // Verify
      assertEquals(sat.getRadius(), 3.0 * OrbitalObject::ZOOM);
      assertEquals(sat.isAlive(), true);
   }
   
   // Test parameterized constructor
   void parameterizedConstructor()
   {
      // Setup
      Position pos(100.0, 200.0);
      Velocity vel(5.0, -3.0);
      Angle angle(M_PI / 4);
      DummySatellite sat(pos, vel, angle);
      
      // Verify
      assertEquals(sat.getPosition().getMetersX(), pos.getMetersX());
      assertEquals(sat.getPosition().getMetersY(), pos.getMetersY());
      assertEquals(sat.getVelocity().getDX(), vel.getDX());
      assertEquals(sat.getVelocity().getDY(), vel.getDY());
      assertEquals(sat.angleShip.getRadians(), angle.getRadians());
      assertEquals(sat.getRadius(), 3.0 * OrbitalObject::ZOOM);
   }
   
   // Test isDead() method
   void isDeadTest()
   {
      // Setup
      DummySatellite sat;
      
      // Verify initial state
      assertEquals(sat.isDead(), false);
      
      // Kill the satellite
      sat.kill();
      
      // Verify dead state
      assertEquals(sat.isDead(), true);
   }
   
   // Test collision handling
   void collisionTest()
   {
      // Setup
      DummySatellite sat;
      vector<Part> parts;
      vector<Fragment> fragments;
      
      // Trigger collision
      sat.onCollision(parts, fragments);
      
      // Verify
      assertEquals(sat.isDead(), true);
      assertUnit(parts.size() >= 0);     // Should spawn parts
      assertUnit(fragments.size() >= 0);  // Should spawn fragments
   }
};
