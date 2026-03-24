/***********************************************************************
 * Header File:
 *    TEST ANGLE
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    All the unit tests for ANGLE
 ************************************************************************/

#pragma once


#include "angle.h"
#include "unitTest.h"

/*******************************
 * TEST ANGLE
 * A friend class for Angle which contains the Angle unit tests
 ********************************/
class TestAngle : public UnitTest
{
public:
   void run()
   {
      // Ticket 1: From before
      defaultConstructor();
      setUp();
      setDown();
      setRight();
      setLeft();
      reverse();
      setRadians_noNormalize();
      setDegrees_noNormalize();
      getDegrees_0();
      getDegrees_270();
      getRadians_0();
      getRadians_270();
      add_0();
      add_value();
      setRadians_negative();
      setRadians_oneLap();
      setRadians_sixLaps();
      setRadians_negativeThreeQuarters();
      setRadians_negativeOneLap();
      setRadians_negativeSixLaps();
      setDegrees_negative();
      setDegrees_oneLap();
      setDegrees_sixLaps();
      setDegrees_negativeThreeQuarters();
      setDegrees_negativeOneLap();
      setDegrees_negativeSixLaps();
      add_positiveLap();
      add_negativeLap();
      
      // Ticket 2: Components
      getDx_up();
      getDx_down();
      getDx_left();
      getDx_right();
      getDx_diagonal();
      getDy_up();
      getDy_down();
      getDy_left();
      getDy_right();
      getDy_diagonal();
      isRight_right();
      isRight_left();
      isLeft_right();
      isLeft_left();
      setDxDy_up();
      setDxDy_right();
      setDxDy_left();
      setDxDy_diagonal();
      
      report("Angle");
   }
   
private:
   
   /*****************************************************************
    *****************************************************************
    * CONSTRUCTOR
    *****************************************************************
    *****************************************************************/
   
   
   /*********************************************
    * name:    DEFAULT CONSTRUCTOR
    * input:   nothing
    * output:  zero
    *********************************************/
   void defaultConstructor()
   {  // setup
      // exercise
      Angle a;
      // verify
      assertUnit(a.radians == 0.0);
   }  // teardown
   
   /*****************************************************************
    *****************************************************************
    * SETTERS
    *****************************************************************
    *****************************************************************/
   
   /*********************************************
    * name:    SET UP
    * input:   nothing
    * output:  0 degrees
    *********************************************/
   void setUp()
   {  // setup
      Angle a;
      a.radians = -99.9;
      // exercise
      a.setUp();
      // verify
      assertEquals(a.radians, 0.0);
   }  // teardown
   
   /*********************************************
    * name:    SET DOWN
    * input:   nothing
    * output:  180 degrees
    *********************************************/
   void setDown()
   {  // setup
      Angle a;
      a.radians = -99.9;
      // exercise
      a.setDown();
      // verify
      assertEquals(a.radians, M_PI);
   }  // teardown
   
   /*********************************************
    * name:    SET RIGHT
    * input:   nothing
    * output:  90 degrees
    *********************************************/
   void setRight()
   {  // setup
      Angle a;
      a.radians = -99.9;
      // exercise
      a.setRight();
      // verify
      assertEquals(a.radians, M_PI_2);
   }  // teardown
   
   /*********************************************
    * name:    SET LEFT
    * input:   nothing
    * output:  270 degrees
    *********************************************/
   void setLeft()
   {  // setup
      Angle a;
      a.radians = -99.9;
      // exercise
      a.setLeft();
      // verify
      assertEquals(a.radians, M_PI + M_PI_2);
   }  // teardown
   
   /*********************************************
    * name:    REVERSE
    * input:   90
    * output:  270 degrees
    *********************************************/
   void reverse()
   {  // setup
      Angle a;
      a.radians = M_PI_2; // 90 degrees
                          // exercise
      a.reverse();
      // verify
      assertEquals(a.radians, M_PI_2 + M_PI);
   }  // teardown
   
   /*********************************************
    * name:    SET RADIANS - NO NORMALIZATION
    * input:   45 degrees
    * output:  45 degrees
    *********************************************/
   void setRadians_noNormalize()
   {  // setup
      Angle a;
      a.radians = -99.9;
      // exercise
      a.setRadians(0.785398);
      // verify
      assertEquals(a.radians, 0.785398);
   }  // teardown
   
   /*********************************************
    * name:    SET RADIANS - NEGATIVE
    * input:   -90
    * output:  270 degrees
    *********************************************/
   void setRadians_negative()
   {  // setup
      Angle a;
      a.radians = -99.9;
      // exercise
      a.setRadians(-M_PI_2);
      // verify
      assertEquals(a.radians, M_PI + M_PI_2);
   }  // teardown
   
   /*********************************************
    * name:    SET RADIANS - ONE LAP
    * input:   45 + 360 degrees
    * output:  45 degrees
    *********************************************/
   void setRadians_oneLap()
   {  // setup
      Angle a;
      a.radians = -99.9;
      // exercise
      a.setRadians(0.785398 + (M_PI * 2.0));
      // verify
      assertEquals(a.radians, 0.785398);
   }  // teardown
   
   /*********************************************
    * name:    SET RADIANS - SIX LAP
    * input:   45 + (6 x 360) degrees
    * output:  45 degrees
    *********************************************/
   void setRadians_sixLaps()
   {  // setup
      Angle a;
      a.radians = -99.9;
      // exercise
      a.setRadians(0.785398 + (M_PI * 2.0) * 6.0);
      // verify
      assertEquals(a.radians, 0.785398);
   }  // teardown
   
   /*********************************************
    * name:    SET RADIANS - NEGATIVE 3/4
    * input:   -270 degrees
    * output:  90 degrees
    *********************************************/
   void setRadians_negativeThreeQuarters()
   {  // setup
      Angle a;
      a.radians = -99.9;
      // exercise
      a.setRadians(-M_PI - M_PI_2);
      // verify
      assertEquals(a.radians, M_PI_2);
   }  // teardown
   
   /*********************************************
    * name:    SET RADIANS - NEGATIVE ONE LAP
    * input:   -45 - 360 degrees
    * output:  315 degrees
    *********************************************/
   void setRadians_negativeOneLap()
   {  // setup
      Angle a;
      a.radians = -99.9;
      // exercise
      a.setRadians(-0.785398 - (M_PI * 2.0));
      // verify
      assertEquals(a.radians, 5.49779);
   }  // teardown
   
   /*********************************************
    * name:    SET RADIANS - NEGATIVE SIX LAP
    * input:   -45 - 360*6 degrees
    * output:  315 degrees
    *********************************************/
   void setRadians_negativeSixLaps()
   {  // setup
      Angle a;
      a.radians = -99.9;
      // exercise
      a.setRadians(-0.785398 - (M_PI * 2.0) * 6.0);
      // verify
      assertEquals(a.radians, 5.49779);
   }  // teardown
   
   /*********************************************
    * name:    SET DEGREES - NO NORMALIZATION
    * input:   45 degrees
    * output:  45 degrees
    *********************************************/
   void setDegrees_noNormalize()
   {  // setup
      Angle a;
      a.radians = -99.9;
      // exercise
      a.setDegrees(45.0);
      // verify
      assertEquals(a.radians, 0.785398);
   }  // teardown
   
   /*********************************************
    * name:    SET DEGREES - NEGATIVE
    * input:   -90
    * output:  270 degrees
    *********************************************/
   void setDegrees_negative()
   {  // setup
      Angle a;
      a.radians = -99.9;
      // exercise
      a.setDegrees(-90.0);
      // verify
      assertEquals(a.radians, M_PI + M_PI_2);
   }  // teardown
   
   /*********************************************
    * name:    SET DEGREES - ONE LAP
    * input:   45 + 360 degrees
    * output:  45 degrees
    *********************************************/
   void setDegrees_oneLap()
   {  // setup
      Angle a;
      a.radians = -99.9;
      // exercise
      a.setDegrees(45.0 + 360.0);
      // verify
      assertEquals(a.radians, 0.785398);
   }  // teardown
   
   /*********************************************
    * name:    SET DEGREES - SIX LAP
    * input:   45 + (6 x 360) degrees
    * output:  45 degrees
    *********************************************/
   void setDegrees_sixLaps()
   {  // setup
      Angle a;
      a.radians = -99.9;
      // exercise
      a.setDegrees(45.0 + 360.0 * 6.0);
      // verify
      assertEquals(a.radians, 0.785398);
   }  // teardown
   
   /*********************************************
    * name:    SET DEGREES - NEGATIVE 3/4
    * input:   -270 degrees
    * output:  90 degrees
    *********************************************/
   void setDegrees_negativeThreeQuarters()
   {  // setup
      Angle a;
      a.radians = -99.9;
      // exercise
      a.setDegrees(-270.0);
      // verify
      assertEquals(a.radians, M_PI_2);
   }  // teardown
   
   /*********************************************
    * name:    SET DEGREES - NEGATIVE ONE LAP
    * input:   -45 - 360 degrees
    * output:  315 degrees
    *********************************************/
   void setDegrees_negativeOneLap()
   {  // setup
      Angle a;
      a.radians = -99.9;
      // exercise
      a.setDegrees(-45.0 - 360.0);
      // verify
      assertEquals(a.radians, 5.49779);
   }  // teardown
   
   /*********************************************
    * name:    SET DEGREES - NEGATIVE SIX LAP
    * input:   -45 - 360*6 degrees
    * output:  315 degrees
    *********************************************/
   void setDegrees_negativeSixLaps()
   {  // setup
      Angle a;
      a.radians = -99.9;
      // exercise
      a.setDegrees(-45.0 - 360.0 * 6.0);
      // verify
      assertEquals(a.radians, 5.49779);
   }  // teardown
   
   /*****************************************************************
    *****************************************************************
    * ADD
    *****************************************************************
    *****************************************************************/
   
   /*********************************************
    * name:    ADD ZERO
    * input:   (45 degrees) 0
    * output:  45 degrees
    *********************************************/
   void add_0()
   {  // setup
      Angle a;
      a.radians = 0.785398;
      double r = 0.0;
      
      // exercise
      a.add(r);
      
      // verify
      assertEquals(a.radians, 0.785398);
      assertEquals(r, 0.0);
   }  // teardown
   
   /*********************************************
    * name:    ADD VALUE
    * input:   (45 degrees) 180 degrees
    * output:  45 degrees
    *********************************************/
   void add_value()
   {  // setup
      Angle a;
      a.radians = 0.785398;
      double r = M_PI;
      
      // exercise
      a.add(r);
      
      // verify
      assertEquals(a.radians, 0.785398 + M_PI);
      assertEquals(r, M_PI);
   }  // teardown
   
   /*********************************************
    * name:    ADD POSITIVE ONE LAP
    * input:   (45 degrees) 180 + 360 degrees
    * output:  45 degrees
    *********************************************/
   void add_positiveLap()
   {  // setup
      Angle a;
      a.radians = 0.785398;
      double r = M_PI + M_PI * 2.0;
      
      // exercise
      a.add(r);
      
      // verify
      assertEquals(a.radians, 0.785398 + M_PI);
      assertEquals(r, M_PI + M_PI * 2.0);
   }  // teardown
   
   /*********************************************
    * name:    ADD NEGATIVE ONE LAP
    * input:   (45 degrees) -90 degrees
    * output:  315 degrees
    *********************************************/
   void add_negativeLap()
   {  // setup
      Angle a;
      a.radians = 0.785398;
      double r = -M_PI_2;
      
      // exercise
      a.add(r);
      
      // verify
      assertEquals(a.radians, 5.49779);
      assertEquals(r, -M_PI_2);
   }  // teardown
   
   /*********************************************
    * name:    SET DX DY  UP
    * input:   dx=0 dy=1
    * output:  0 degrees
    *********************************************/
   void setDxDy_up()
   {  // setup
      Angle a;
      a.radians = -99.9;
      double dx = 0;
      double dy = 1;
      
      // exercise
      a.setDxDy(dx, dy);
      
      // verify
      assertEquals(a.radians, 0);
   }  // teardown
   
   
   /*********************************************
    * name:    SET DX DY  RIGHT
    * input:   dx=4 dy=0
    * output:  90 degrees
    *********************************************/
   void setDxDy_right()
   {  // setup
      Angle a;
      a.radians = -99.9;
      double dx = 4;
      double dy = 0;
      
      // exercise
      a.setDxDy(dx, dy);
      
      // verify
      assertEquals(a.radians, M_PI_2);
   }  // teardown
   
   /*********************************************
    * name:    SET DX DY  LEFT
    * input:   dx=-0.01 dy=0
    * output:  270 degrees
    *********************************************/
   void setDxDy_left()
   {  // setup
      Angle a;
      a.radians = -99.9;
      double dx = -0.01;
      double dy = 0;
      
      // exercise
      a.setDxDy(dx, dy);
      
      // verify
      assertEquals(a.radians, M_PI + M_PI_2);
   }  // teardown
   
   /*********************************************
    * name:    SET DX DY  DIAGONAL
    * input:   dx= 5.0 dy=8.6602
    * output:  30 degrees
    *********************************************/
   void setDxDy_diagonal()
   {  // setup
      Angle a;
      a.radians = 1.234567;
      double dx = 5.0;
      double dy = 8.6602;
      
      // exercise
      a.setDxDy(dx, dy);
      
      // verify
      assertEquals(a.radians, M_PI / 6.0);
   }  // teardown
   
   
   /*****************************************************************
    *****************************************************************
    * GETTERS
    *****************************************************************
    *****************************************************************/
   
   /*********************************************
    * name:    GET DEGREES - 0
    * input:   0 degrees
    * output:  0 degrees
    *********************************************/
   void getDegrees_0()
   {  // setup
      Angle a;
      a.radians = 0.0;
      double d = -99.9;
      // exercise
      d = a.getDegrees();
      // verify
      assertEquals(d, 0.0);
      assertEquals(a.radians, 0.0);
   }  // teardown
   
   /*********************************************
    * name:    GET DEGREES - 270
    * input:   270 degrees
    * output:  270 degrees
    *********************************************/
   void getDegrees_270()
   {  // setup
      Angle a;
      a.radians = M_PI + M_PI_2;
      double d = -99.9;
      // exercise
      d = a.getDegrees();
      // verify
      assertEquals(d, 270.0);
      assertEquals(a.radians, M_PI + M_PI_2);
   }  // teardown
   
   /*********************************************
    * name:    GET RADIANS - 0
    * input:   0 degrees
    * output:  0 degrees
    *********************************************/
   void getRadians_0()
   {  // setup
      Angle a;
      a.radians = 0.0;
      double r = -99.9;
      // exercise
      r = a.getRadians();
      // verify
      assertEquals(r, 0.0);
      assertEquals(a.radians, 0.0);
   }  // teardown
   
   /*********************************************
    * name:    GET RADIANS - 270
    * input:   270 degrees
    * output:  270 degrees
    *********************************************/
   void getRadians_270()
   {  // setup
      Angle a;
      a.radians = M_PI + M_PI_2;
      double r = -99.9;
      // exercise
      r = a.getRadians();
      // verify
      assertEquals(r, M_PI + M_PI_2);
      assertEquals(a.radians, M_PI + M_PI_2);
   }  // teardown
   
   /*********************************************
    * name:    GET DX - UP
    * input:   0 degrees
    * output:  0
    *********************************************/
   void getDx_up()
   {  // setup
      Angle a;
      a.radians = 0; // 0 degrees
      double dx = -99.9;
      // exercise
      dx = a.getDx();
      // verify
      assertEquals(dx, 0);
      assertEquals(a.radians, 0);
   }  // teardown
   
   /*********************************************
    * name:    GET DX - DOWN
    * input:   180 degrees
    * output:  0
    *********************************************/
   void getDx_down()
   {  // setup
      Angle a;
      a.radians = M_PI; // 180 degrees
      double dx = -99.9;
      // exercise
      dx = a.getDx();
      // verify
      assertEquals(dx, 0);
      assertEquals(a.radians, M_PI);
   }  // teardown
   
   
   /*********************************************
    * name:    GET DX - LEFT
    * input:   270 degrees
    * output:  -1
    *********************************************/
   void getDx_left()
   {  // setup
      Angle a;
      a.radians = M_PI + M_PI_2; // 270 degrees
      double dx = -99.9;
      // exercise
      dx = a.getDx();
      // verify
      assertEquals(dx, -1);
      assertEquals(a.radians, M_PI + M_PI_2);
   }  // teardown
   
   /*********************************************
    * name:    GET DX - RIGHT
    * input:   90 degrees
    * output:  1
    *********************************************/
   void getDx_right()
   {  // setup
      Angle a;
      a.radians = M_PI / 6.0; // 90 degrees
      double dx = -99.9;
      // exercise
      dx = a.getDx();
      // verify
      assertEquals(dx, 0.5);
      assertEquals(a.radians, M_PI / 6.0);
   }  // teardown
   
   /*********************************************
    * name:    GET DX - DIAGONAL
    * input:   30 degrees
    * output:  0.5
    *********************************************/
   void getDx_diagonal()
   {  // setup
      Angle a;
      a.radians = M_PI / 6.0; // 30 degrees
      double dx = -99.9;
      // exercise
      dx = a.getDx();
      // verify
      assertEquals(dx, 0.5);
      assertEquals(a.radians, M_PI / 6.0);
   }  // teardown
   
   
   /*********************************************
    * name:    GET DY - UP
    * input:   0 degrees
    * output:  1
    *********************************************/
   void getDy_up()
   {  // setup
      Angle a;
      a.radians = 0; // 0 degrees
      double dy = -99.9;
      // exercise
      dy = a.getDy();
      // verify
      assertEquals(dy, 1);
      assertEquals(a.radians, 0);
   }  // teardown
   
   /*********************************************
    * name:    GET DY - DOWN
    * input:   180 degrees
    * output:  -1
    *********************************************/
   void getDy_down()
   {  // setup
      Angle a;
      a.radians = M_PI; // 180 degrees
      double dy = -99.9;
      // exercise
      dy = a.getDy();
      // verify
      assertEquals(dy, -1);
      assertEquals(a.radians, M_PI);
   }  // teardown
   
   
   /*********************************************
    * name:    GET DY - LEFT
    * input:   270 degrees
    * output:  0
    *********************************************/
   void getDy_left()
   {  // setup
      Angle a;
      a.radians = M_PI + M_PI_2; // 270 degrees
      double dy = -99.9;
      // exercise
      dy = a.getDy();
      // verify
      assertEquals(dy, 0);
      assertEquals(a.radians, M_PI + M_PI_2);
   }  // teardown
   
   /*********************************************
    * name:    GET DY - RIGHT
    * input:   90 degrees
    * output:  0
    *********************************************/
   void getDy_right()
   {  // setup
      Angle a;
      a.radians = M_PI_2; // 90 degrees
      double dy = -99.9;
      // exercise
      dy = a.getDy();
      // verify
      assertEquals(dy, 0);
      assertEquals(a.radians, M_PI_2);
   }  // teardown
   
   /*********************************************
    * name:    GET DY - DIAGONAL
    * input:   30 degrees
    * output:  0.5
    *********************************************/
   void getDy_diagonal()
   {  // setup
      Angle a;
      a.radians = M_PI / 6.0; // 30 degrees
      double dy = -99.9;
      // exercise
      dy = a.getDy();
      // verify
      assertEquals(dy, 0.866025);
      assertEquals(a.radians, M_PI / 6.0);
   }  // teardown
   
   /*********************************************
    * name:    IS RIGHT - right
    * input:   30 degrees
    * output:  true
    *********************************************/
   void isRight_right()
   { // setup
      Angle a;
      a.radians = M_PI / 6.0; // 30 degrees
      bool isRight = false;
      
      // exercise
      isRight = a.isRight();
      
      // verify
      assertEquals(isRight, true);
      assertEquals(a.radians, M_PI / 6.0);
   } // teardown
   
   /*********************************************
    * name:    IS RIGHT - LEFT
    * input:   330 degrees
    * output:  false
    *********************************************/
   void isRight_left()
   { // setup
      Angle a;
      a.radians = 11 * M_PI / 6; // 330 degrees
      bool isRight = true;
      
      // exercise
      isRight = a.isRight();
      
      // verify
      assertEquals(isRight, false);
      assertEquals(a.radians, 11 * M_PI / 6.0);
   } // teardown
   
   /*********************************************
    * name:    IS LEFT - right
    * input:   30 degrees
    * output:  false
    *********************************************/
   void isLeft_right()
   { // setup
      Angle a;
      a.radians = M_PI / 6; // 30 degrees
      bool isLeft = true;
      
      // exercise
      isLeft = a.isLeft();
      
      // verify
      assertEquals(isLeft, false);
      assertEquals(a.radians, M_PI / 6);
   } // teardown
   
   /*********************************************
    * name:    IS LEFT - LEFT
    * input:   330 degrees
    * output:  true
    *********************************************/
   void isLeft_left()
   { // setup
      Angle a;
      a.radians = 11 * M_PI / 6; // 330 degrees
      bool isLeft = false;
      
      // exercise
      isLeft = a.isLeft();
      
      // verify
      assertEquals(isLeft, true);
      assertEquals(a.radians, 11 * M_PI / 6);
   } // teardown
   
};
