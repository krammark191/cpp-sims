/***********************************************************************
 * Header File:
 *    TEST DREAMCHASER
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Unit tests for DreamChaser, in robust single-case style
 ************************************************************************/

#pragma once

#include "unitTest.h"
#include "DreamChaser.h"
#include <cmath>

/***************************************************
 * TEST DREAMCHASER
 * Test the DreamChaser class
 ***************************************************/
class TestDreamChaser : public UnitTest
{
public:
   void run()
   {
      rotateLeft_fromZero();
      rotateLeft_fromNearZero();
      rotateRight_fromZero();
      rotateRight_fromNear360();
      thrust_changesVelocity();
      update_changesPosition();
      report("DreamChaser");
   }
   
private:
   const double ROTATION_AMOUNT = 0.1 * 180.0 / M_PI; // ≈ 5.72957795
   const double FULL_CIRCLE = 360.0;
   const double EPSILON = 0.001;
   
   /*********************************************
    * ROTATE LEFT FROM ZERO
    * input: angle=0
    * output: angle=354.3
    *********************************************/
   void rotateLeft_fromZero()
   {
      DummyDreamChaser dc;
      dc.setAngleShip(0.0); // degrees
      
      dc.rotateLeft();
      
      double expected = FULL_CIRCLE - ROTATION_AMOUNT;
      double actual = dc.getAngleShip();
      assertEqualsTolerance(actual, expected, EPSILON);
      assertUnit(dc.rotateLeftCalls == 1);
   }
   
   /*********************************************
    * ROTATE LEFT FROM NEAR ZERO
    * input: angle=2
    * output: angle=356.3
    *********************************************/
   void rotateLeft_fromNearZero()
   {
      DummyDreamChaser dc;
      dc.setAngleShip(2.0); // degrees
      
      dc.rotateLeft();
      
      double expected = fmod(2.0 - ROTATION_AMOUNT + FULL_CIRCLE, FULL_CIRCLE);
      double actual = dc.getAngleShip();
      assertEqualsTolerance(actual, expected, EPSILON);
      assertUnit(dc.rotateLeftCalls == 1);
   }
   
   /*********************************************
    * ROTATE RIGHT FROM ZERO
    * input: angle=0
    * output: angle=5.7
    *********************************************/
   void rotateRight_fromZero()
   {
      DummyDreamChaser dc;
      dc.setAngleShip(0.0); // degrees
      
      dc.rotateRight();
      
      double expected = ROTATION_AMOUNT;
      double actual = dc.getAngleShip();
      assertEqualsTolerance(actual, expected, EPSILON);
      assertUnit(dc.rotateRightCalls == 1);
   }
   
   /*********************************************
    * ROTATE RIGHT FROM NEAR 360
    * input: angle=358
    * output: angle=3.7
    *********************************************/
   void rotateRight_fromNear360()
   {
      DummyDreamChaser dc;
      dc.setAngleShip(358.0); // degrees
      
      dc.rotateRight();
      
      double expected = fmod(358.0 + ROTATION_AMOUNT, FULL_CIRCLE);
      double actual = dc.getAngleShip();
      assertEqualsTolerance(actual, expected, EPSILON);
      assertUnit(dc.rotateRightCalls == 1);
   }
   
   /*********************************************
    * THRUST CHANGES VELOCITY
    *********************************************/
   void thrust_changesVelocity()
   {
      DummyDreamChaser dc;
      double dxBefore = dc.getVelocity().getDX();
      double dyBefore = dc.getVelocity().getDY();
      
      dc.thrust(1.0);
      
      bool changed = (dc.getVelocity().getDX() != dxBefore) ||
      (dc.getVelocity().getDY() != dyBefore);
      assertUnit(dc.thrustCalls == 1);
      assertUnit(changed);
   }
   
   /*********************************************
    * UPDATE CHANGES POSITION
    *********************************************/
   void update_changesPosition()
   {
      DummyDreamChaser dc;
      double xBefore = dc.getPosition().getMetersX();
      double yBefore = dc.getPosition().getMetersY();
      
      dc.update(1.0);
      
      bool changed = (dc.getPosition().getMetersX() != xBefore) ||
      (dc.getPosition().getMetersY() != yBefore);
      assertUnit(dc.updateCalls == 1);
      assertUnit(changed);
   }
};
