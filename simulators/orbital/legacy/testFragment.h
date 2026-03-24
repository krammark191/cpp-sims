/***********************************************************************
 * Header File:
 *    Test Fragment : Unit tests for Fragment
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Unit tests for Fragment class
 ************************************************************************/

#pragma once

#include "fragment.h"

/*********************************
 * TEST FRAGMENT
 * A friend class for Fragment which contains the unit tests
 *********************************/
class TestFragment : public UnitTest
{
public:
   void run()
   {
      test_defaultConstructor();
      test_nonDefaultConstructor();
      test_getRadius();
      test_update();
      test_onCollision();
      test_angularVelocity();
      
      report("Fragment");
   }
   
private:
   void test_defaultConstructor()
   {
      // setup
      DummyFragment fragment;
      Position positionDefault;
      Velocity velocityDefault;
      Angle angleDefault;
      
      // exercise
      Position positionFragment = fragment.getPosition();
      Velocity velocityFragment = fragment.getVelocity();
      Angle angleFragment = fragment.getAngleShip();
      
      // verify
      assertEquals(positionFragment.getMetersX(), positionDefault.getMetersX());
      assertEquals(positionFragment.getMetersY(), positionDefault.getMetersY());
      assertEquals(velocityFragment.getDX(), velocityDefault.getDX());
      assertEquals(velocityFragment.getDY(), velocityDefault.getDY());
      assertEquals(angleFragment.getRadians(), angleDefault.getRadians());
      assertEquals(fragment.getRadius(), 2.0 * OrbitalObject::ZOOM);
   }  // cleanup
   
   void test_nonDefaultConstructor()
   {
      // setup
      Position pos(100.0, 200.0);
      Velocity vel(5.0, -3.0);
      int testLifespan = 75;
      
      // exercise
      DummyFragment fragment(pos, vel, testLifespan);
      
      // verify
      assertEquals(fragment.getPosition().getMetersX(), pos.getMetersX());
      assertEquals(fragment.getPosition().getMetersY(), pos.getMetersY());
      assertEquals(fragment.getVelocity().getDX(), vel.getDX());
      assertEquals(fragment.getVelocity().getDY(), vel.getDY());
      assertEquals(fragment.getRadius(), 2.0 * OrbitalObject::ZOOM);
   }  // cleanup
   
   void test_getRadius()
   {
      // setup
      DummyFragment fragment;
      double expectedRadius = 2.0 * OrbitalObject::ZOOM;
      
      // exercise
      double radius = fragment.getRadius();
      
      // verify
      assertEquals(radius, expectedRadius);
   }  // cleanup
   
   void test_update()
   {
      // setup
      DummyFragment fragment;
      int initialFrames = 0;
      
      // exercise
      fragment.update(1.0);
      
      // verify
      assertEquals(fragment.framesElapsed, initialFrames + 1);
      assertEquals(fragment.isAlive(), true);
      
      // setup for death
      fragment.framesElapsed = fragment.lifespan;
      
      // exercise
      fragment.update(1.0);
      
      // verify
      assertEquals(fragment.isAlive(), false);
   }  // cleanup
   
   void test_onCollision()
   {
      // setup
      DummyFragment fragment;
      vector<Part> parts;
      vector<Fragment> fragments;
      
      // exercise
      fragment.onCollision(parts, fragments);
      
      // verify
      assertEquals(fragment.isAlive(), false);
      assertEquals(parts.size(), (size_t)0);    // No new parts
      assertEquals(fragments.size(), (size_t)0); // No new fragments
   }  // cleanup
   
   void test_angularVelocity()
   {
      // setup
      DummyFragment fragment;
      
      // exercise
      double angularVel = fragment.getAngularVelocity();
      
      // verify
      assertEquals(angularVel >= -0.5 && angularVel <= 0.5, true);
   }  // cleanup
};
