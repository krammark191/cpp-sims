/***********************************************************************
 * Header File:
 *    TEST ORBITAL OBJECT
 * Author:
 *    Mark Van Horn & Taden Marston
 * Summary:
 *    Unit tests for the Orbital Object class.
 ************************************************************************/

#pragma once

#include "unitTest.h"
#include "OrbitalObject.h"
#include "position.h"
#include "velocity.h"

/*********************************************
 * TEST ORBITAL OBJECT
 * Unit tests for the OrbitalObject base class.
 *********************************************/
class TestOrbitalObject : public UnitTest
{
private:
   // Dummy derived class to test OrbitalObject's concrete methods
   class DummyOrbitalObject : public OrbitalObject
   {
   public:
      DummyOrbitalObject() : OrbitalObject() { }
      DummyOrbitalObject(const Position& p, const Velocity& v, const Angle& a) : OrbitalObject(p, v, a) { }
      void update(double time) override { } /* Do nothing for base class testing */
      void draw(ogstream& gout) override { } /* Do nothing for base class testing */
   };

public:
   void run()
   {
      // Constructors
      constructor_default();
      constructor_nonDefault();

      // Getters
      getPosition();
      getVelocity();
      getHeight();
      isAlive_default();
      isAlive_killed();

      // Setters
      setPosition();
      setVelocity();
      kill();

      report("OrbitalObject");
   }

private:
   // Test cases for OrbitalObject
   void constructor_default()
   {
      // Setup
      DummyOrbitalObject obj;
      // Verify
      assertEquals(obj.getPosition().getMetersX(), 0.0);
      assertEquals(obj.getPosition().getMetersY(), 0.0);
      assertEquals(obj.getVelocity().getDX(), 0.0);
      assertEquals(obj.getVelocity().getDY(), 0.0);
      assertEquals(obj.isAlive(), true);
   }

   void constructor_nonDefault()
   {
      // Setup
      Position p(100.0, 200.0);
      Velocity v(10.0, 20.0);
      Angle a(30.0); // Example angle
      DummyOrbitalObject obj(p, v, a);
      // Verify
      assertEquals(obj.getPosition().getMetersX(), 100.0);
      assertEquals(obj.getPosition().getMetersY(), 200.0);
      assertEquals(obj.getVelocity().getDX(), 10.0);
      assertEquals(obj.getVelocity().getDY(), 20.0);
      assertEquals(obj.isAlive(), true);
   }

   void getPosition()
   {
      // Setup
      Position p(100.0, 200.0);
      Velocity v(10.0, 20.0);
      Angle a(30.0); // Example angle
      DummyOrbitalObject obj(p, v, a);
      // Exercise
      Position retrievedPos = obj.getPosition();
      // Verify
      assertEquals(retrievedPos.getMetersX(), 100.0);
      assertEquals(retrievedPos.getMetersY(), 200.0);
   }

   void getVelocity()
   {
      // Setup
      Position p(100.0, 200.0);
      Velocity v(10.0, 20.0);
      Angle a(30.0); // Example angle
      DummyOrbitalObject obj(p, v, a);
      // Exercise
      Velocity retrievedVel = obj.getVelocity();
      // Verify
      assertEquals(retrievedVel.getDX(), 10.0);
      assertEquals(retrievedVel.getDY(), 20.0);
   }

   void getHeight()
   {
      // Setup
      Position p(100.0, 200.0);
      Velocity v(10.0, 20.0);
      Angle a(30.0); // Example angle
      DummyOrbitalObject obj(p, v, a);
      // Exercise
      double retrievedHeight = obj.getHeight();
      // Verify
      assertEquals(retrievedHeight, -6377776.39320);
   }

   void isAlive_default()
   {
      // Setup
      DummyOrbitalObject obj;
      // Exercise & Verify
      assertEquals(obj.isAlive(), true);
   }

   void isAlive_killed()
   {
      // Setup
      DummyOrbitalObject obj;
      // Exercise
      obj.kill();
      // Verify
      assertEquals(obj.isAlive(), false);
   }

   void setPosition()
   {
      // Setup
      DummyOrbitalObject obj;
      Position newP(50.0, 60.0);
      // Exercise
      obj.setPosition(newP);
      // Verify
      assertEquals(obj.getPosition().getMetersX(), 50.0);
      assertEquals(obj.getPosition().getMetersY(), 60.0);
   }

   void setVelocity()
   {
      // Setup
      DummyOrbitalObject obj;
      Velocity newV(5.0, 6.0);
      // Exercise
      obj.setVelocity(newV);
      // Verify
      assertEquals(obj.getVelocity().getDX(), 5.0);
      assertEquals(obj.getVelocity().getDY(), 6.0);
   }

   void kill()
   {
      // Setup
      DummyOrbitalObject obj;
      // Exercise
      obj.kill();
      // Verify
      assertEquals(obj.isAlive(), false);
   }
};
