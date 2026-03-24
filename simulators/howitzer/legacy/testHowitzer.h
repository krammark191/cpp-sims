/***********************************************************************
 * Header File:
 *    TEST HOWITZER
 * Author:
 *    Mark Van Horn & Taden Marston
 * Summary:
 *    All the unit tests for Howitzer
 ************************************************************************/


#pragma once

#include "howitzer.h"
#include "unitTest.h"

/*******************************
 * TEST HOWITZER
 * A friend class for Howitzer which contains the Howitzer unit tests
 ********************************/
class TestHowitzer : public UnitTest
{
public:
   void run()
   {
      // Ticket 1: Getters
      defaultConstructor();
      getPosition_zero();
      getPosition_middle();
      getMuzzleVelocity_slow();
      getMuzzleVelocity_standard();
      getElevation_up();
      getElevation_right();
      getElevation_left();
      
      // Ticket 2: Setters
      generatePosition_small();
      generatePosition_large();
      raise_rightDown();
      raise_rightUp();
      raise_leftDown();
      raise_leftUp();
      rotate_clock();
      rotate_counterClock();
      rotate_wrapClock();
      rotate_wrapCounterClock();
      
      report("Howitzer");
   }
   
private:
   double metersFromPixels = -1.0;
   
   /*****************************************************************
    *****************************************************************
    * CONSTRUCTOR
    *****************************************************************
    *****************************************************************/
   
   /*********************************************
    * name:    DEFAULT CONSTRUCTOR
    * input:   nothing
    * output:  zeros (except angle is at 45 degrees,
    *                 and the muzzle velocity is correct)
    *********************************************/
   void defaultConstructor()
   {  // setup
      Howitzer h;
      
      // exercise
      Position pos = h.getPosition();
      double angle = h.getElevation().getDegrees();
      double muzzleVelocity = h.getMuzzleVelocity();
      
      // verify
      assertUnit(pos.getMetersX() == 0.0);
      assertUnit(pos.getMetersY() == 0.0);
      assertUnit(angle == 45.0);
      assertUnit(muzzleVelocity == 827.0);
      
      // teardown
      // (nothing to teardown)
   }
   
   /*****************************************************************
    *****************************************************************
    * GETTERS
    *****************************************************************
    *****************************************************************/
   
   /*********************************************
    * name:    GET POSITION ZERO
    * input:   h.pos=(0,0)
    * output:  pos=(0,0)
    *********************************************/
   void getPosition_zero()
   {
      // setup
      Howitzer h;
      h.position.x = 0.0;
      h.position.y = 0.0;
      
      // exercise
      Position pos = h.getPosition();
      
      // verify
      assertUnit(pos.x == 0.0);
      assertUnit(pos.y == 0.0);
      
      // teardown
      // (nothing to teardown)
   }
   
   /*********************************************
    * name:    GET POSITION MIDDLE
    * input:   h.pos=(123.4, 567.8)
    * output:  pos=(123.4, 567.8)
    *********************************************/
   void getPosition_middle()
   {
      // setup
      Howitzer h;
      h.position.x = 123.4;
      h.position.y = 567.8;
      
      // exercise
      Position pos = h.getPosition();
      
      // verify
      assertUnit(pos.x == 123.4);
      assertUnit(pos.y == 567.8);
      
      // teardown
      // (nothing to teardown)
   }
   
   /*********************************************
    * name:    GET MUZZLE VELOCITY - SLOW SPEED
    * input:   h.muzzleVelocity=(24.68)
    * output:  m=24.68
    *********************************************/
   void getMuzzleVelocity_slow()
   {
      // setup
      Howitzer h;
      h.muzzleVelocity = 24.68;
      
      // exercise
      double muzzleVelocity = h.getMuzzleVelocity();
      
      // verify
      assertUnit(muzzleVelocity == 24.68);
      
      // teardown
      // (nothing to teardown)
   }
   
   /*********************************************
    * name:    GET MUZZLE VELOCITY - STANDARD SPEED
    * input:   h.muzzleVelocity=(827.00)
    * output:  m=827
    *********************************************/
   void getMuzzleVelocity_standard()
   {
      // setup
      Howitzer h;
      h.muzzleVelocity = 827.0;
      
      // exercise
      double muzzleVelocity = h.getMuzzleVelocity();
      
      // verify
      assertUnit(muzzleVelocity == 827.0);
      
      // teardown
      // (nothing to teardown)
   }
   
   /*********************************************
    * name:    GET ELEVATION - up
    * input:   h.elevation=0
    * output:  e=0
    *********************************************/
   void getElevation_up()
   {
      // setup
      Howitzer h;
      h.elevation = 0;
      
      // exercise
      double elevation = h.getElevation().getDegrees();
      
      // verify
      assertUnit(elevation == 0.0);
      
      // teardown
      // (nothing to teardown)
   }
   
   /*********************************************
    * name:    GET ELEVATION - right
    * input:   h.elevation=0.4
    * output:  e=0.4
    *********************************************/
   void getElevation_right()
   {
      // setup
      Howitzer h;
      h.elevation.radians = 0.4;
      
      // exercise
      double elevation = h.getElevation().getRadians();
      
      // verify
      assertUnit(elevation == 0.4);
      
      // teardown
      // (nothing to teardown)
   }
   
   /*********************************************
    * name:    GET ELEVATION - left
    * input:   h.elevation=5.8
    * output:  e=5.8
    *********************************************/
   void getElevation_left()
   {
      // setup
      Howitzer h;
      h.elevation.radians = 5.8;
      
      // exercise
      double elevation = h.getElevation().getRadians();
      
      // verify
      assertUnit(elevation == 5.8);
      
      // teardown
      // (nothing to teardown)
   }
   
   /*****************************************************************
    *****************************************************************
    * SETTERS
    *****************************************************************
    *****************************************************************/
   
   /*********************************************
    * name:    GENERATE POSITION small board
    * input:   (10px, 10px)
    * output:  1px <= x <= 9px
    *********************************************/
   void generatePosition_small()
   {
      // setup
      Howitzer h;
      Position position(10 * 1100, 10 * 1100);
      // exercise
      h.generatePosition(position);
      // verify
      assert(h.position.x / 1100 >= 1 && h.position.x / 1100 <= 9);
      assert(h.position.y == 0);
   }
   
   /*********************************************
    * name:    GENERATE POSITION large board
    * input:   (1000px, 1000px)
    * output:  100px <= x <= 900px
    *********************************************/
   void generatePosition_large()
   {
      // setup
      Howitzer h;
      Position position(1000 * 1100, 1000 * 1100);
      // exercise
      h.generatePosition(position);
      // verify
      assert(h.position.x / 1100 >= 100 && h.position.x / 1100 <= 900);
      assert(h.position.y == 0);
   }
   
   /*********************************************
    * name:    RAISE to the right/down
    * input:   h.elevation=0.5radians  raise(-0.1)
    * output:  h.elevation=0.6radians
    *********************************************/
   void raise_rightDown()
   {
      // setup
      Howitzer h;
      h.elevation.radians = 0.5;
      // exercise
      h.raise(-0.1);
      // verify
      assertEquals(h.elevation.radians, 0.6);
   }
   
   /*********************************************
    * name:    RAISE to the right/up
    * input:   h.elevation=0.5radians  raise(0.1)
    * output:  h.elevation=0.4radians
    *********************************************/
   void raise_rightUp()
   {
      // setup
      Howitzer h;
      h.elevation.radians = 0.5;
      // exercise
      h.raise(0.1);
      // verify
      assertEquals(h.elevation.radians, 0.4);
   }
   
   /*********************************************
    * name:    RAISE to the left down
    * input:   h.elevation=-0.5radians  raise(-0.1)
    * output:  h.elevation=-0.6radians
    *********************************************/
   void raise_leftDown()
   {
      // setup
      Howitzer h;
      h.elevation.radians = M_PI * 2.0 - 0.5;
      // exercise
      h.raise(-0.1);
      // verify
      assertEquals(h.elevation.radians,  M_PI * 2.0 - 0.6);
   }
   
   /*********************************************
    * name:    RAISE to the left up
    * input:   h.elevation=-0.5radians  raise(0.1)
    * output:  h.elevation=-0.4radians
    *********************************************/
   void raise_leftUp()
   {
      // setup
      Howitzer h;
      h.elevation.radians = M_PI * 2.0 - 0.5;
      // exercise
      h.raise(0.1);
      // verify
      assertEquals(h.elevation.radians, M_PI * 2.0 - 0.4);
   }
   
   /*********************************************
    * name:    ROTATE CLOCKWISE no wrapping
    * input:   h.elevation=1.23 rotate=.3
    * output:  h.elevation=1.53
    *********************************************/
   void rotate_clock()
   {
      // setup
      Howitzer h;
      h.elevation.radians = 1.23;
      // exercise
      h.rotate(0.3);
      // verify
      assertEquals(h.elevation.radians, 1.53);
   }
   
   /*********************************************
    * name:    ROTATE COUNTER CLOCKWISE no wrapping
    * input:   h.elevation=1.23 rotate=-.3
    * output:  h.elevation=0.93
    *********************************************/
   void rotate_counterClock()
   {
      // setup
      Howitzer h;
      h.elevation.radians = 1.23;
      // exercise
      h.rotate(-0.3);
      // verify
      assertEquals(h.elevation.radians, 0.93);
   }
   
   /*********************************************
    * name:    ROTATE CLOCKWISE CLOCKWISE WRAP BY 2PI
    * input:   h.elevation=6.1 (2pi + -0.1) rotate=.2
    * output:  h.elevation=.1
    *********************************************/
   void rotate_wrapClock()
   {
      // setup
      Howitzer h;
      h.elevation.radians = 2 * M_PI - 0.1;
      // exercise
      h.rotate(0.2);
      // verify
      assertEquals(h.elevation.radians, 0.1);
   }
   
   /*********************************************
    * name:    ROTATE COUNTER CLOCKWISE WRAP BY 4PI
    * input:   h.elevation=0.1 rotate=-.2 - 4PI
    * output:  h.elevation=-0.1 + 2π
    *********************************************/
   void rotate_wrapCounterClock()
   {
      // setup
      Howitzer h;
      h.elevation.radians = 0.1;
      // exercise
      h.rotate(-0.2 - 4 * M_PI);
      // verify
      assertEquals(h.elevation.radians, 2 * M_PI - 0.1);
   }
   
   /*****************************************************************
    *****************************************************************
    * STANDARD FIXTURE
    *****************************************************************
    *****************************************************************/
   
   // setup standard fixture - set the zoom to 1100m per pixel
   void setupStandardFixture()
   {
      Position p;
      metersFromPixels = p.metersFromPixels;
      p.metersFromPixels = 1100.0;
   }
   
   // teardown the standard fixture - reset the zoom to what it was previously
   void teardownStandardFixture()
   {
      assert(metersFromPixels != -1.0);
      Position p;
      p.metersFromPixels = metersFromPixels;
   }
};
