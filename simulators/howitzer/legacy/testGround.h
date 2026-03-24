/***********************************************************************
 * Header File:
 *    TEST GROUND
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Unit tests for the Ground class.
 ************************************************************************/


#pragma once

#include "ground.h"
#include "unitTest.h"
#include <vector>


using namespace std;

/*******************************
 * TEST GROUND
 * A friend class for Ground which contains the Ground unit tests
 ********************************/
class TestGround : public UnitTest
{
public:
   void run()
   {
      // constructor
      constructor();

      // getters
      getElevationMeters_out();
      getElevationMeters_seven();
      getTarget_two();
      getTarget_seven();
      draw();

      // setter
      reset_ten();

      report("Ground");
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
     * output:  zeros
     *********************************************/
   void constructor()
   {  // setup
      Position posUpperRight;
      double metersFromPixels = posUpperRight.metersFromPixels;
      posUpperRight.metersFromPixels = 1100.0;
      posUpperRight.x = 4400; // 4px
      posUpperRight.y = 5500; // 5px
      // exercise
      Ground g(posUpperRight);
      // verify
      assertUnit(g.iHowitzer == 0);
      assertUnit(g.ground != nullptr);
      assertEquals(g.posUpperRight.x, 4400);
      assertEquals(g.posUpperRight.y, 5500);
      assertEquals(posUpperRight.x, 4400);
      assertEquals(posUpperRight.y, 5500);
      // teardown
      posUpperRight.metersFromPixels = metersFromPixels;
   }  // teardown

   /*****************************************************************
    *****************************************************************
    * GETTERS
    *****************************************************************
    *****************************************************************/


    /*********************************************
     * name:    GET SHELL OUT OF RANGE
     * input:   standard, (-1100, 5500)
     * output:  0.0
     *********************************************/
   void getElevationMeters_out()
   {  // setup
      Ground g;
      setupStandardFixture(g);
      Position pos;
      pos.x = -1100.0;
      pos.y = 5500.0;
      // exercise
      double elevation = g.getElevationMeters(pos);
      // verify
      assertEquals(elevation, 0.0);
      verifyStandardFixture(g);
      // teardown
      teardownStandardFixture(g);
   }  

   /*********************************************
    * name:    GET SHELL SEVEN PIXELS
    * input:   standard, (2200.0, 9900)
    * output:  77000
    *********************************************/
   void getElevationMeters_seven()
   {  // setup
      Ground g;
      setupStandardFixture(g);
      Position pos;
      pos.x = 2200.0;
      pos.y = 9900.0;
      // exercise
      double elevation = g.getElevationMeters(pos);
      // verify
      assertEquals(elevation, 7700.0);  // 7 pixels high or 7700m
      verifyStandardFixture(g);
      // teardown
      teardownStandardFixture(g);
   }  

   /*********************************************
    * name:    GET TARGET LOCATION 2
    * input:   standard, iTarget = 2
    * output:  2200, 7700
    *********************************************/
   void getTarget_two()
   {  // setup
      Position pos;
      Ground g;
      setupStandardFixture(g);
      int iTargetSave = g.iTarget;
      g.iTarget = 2;
      // exercise
      Position posTarget = g.getTarget();
      // verify
      assertEquals(posTarget.x, 2200.0);  // 2px
      assertEquals(posTarget.y, 7700.0);  // 7px
      g.iTarget = iTargetSave;
      verifyStandardFixture(g);
      // teardown
      teardownStandardFixture(g);
   }

   /*********************************************
    * name:    GET TARGET LOCATION 2
    * input:   standard, iTarget = 7
    * output:  7700, 2200
    *********************************************/
   void getTarget_seven()
   {  // setup
      Position pos;
      Ground g;
      setupStandardFixture(g);
      int iTargetSave = g.iTarget;
      g.iTarget = 7;
      // exercise
      Position posTarget = g.getTarget();
      // verify
      assertEquals(posTarget.x, 7700.0);  // 7px
      assertEquals(posTarget.y, 2200.0);  // 2px
      g.iTarget = iTargetSave;
      verifyStandardFixture(g);
      // teardown
      teardownStandardFixture(g);
   }

   // Spy to see exactly what ogstream::draw*() methods are called... and how.
   class ogstreamSpy : public ogstreamDummy
   {
   public:
      // ignore lines
      void drawLine(const Position& begin, const Position& end,
         double red, double green, double blue) { }
      // ignore text
      void drawText(const Position& topLeft, const char* text) { }
      // flush is a no-op
      void flush() { }
      // remember target
      void drawTarget(const Position& pos)
      {
         targets.push_back(pos);
      }
      // remember rectangles
      void drawRectangle(const Position& begin, const Position& end,
         double red, double green, double blue)
      {
         rectanglesBegin.push_back(begin);
         rectanglesEnd.push_back(end);
      }
      vector <Position>  targets;
      vector <Position>  rectanglesBegin;
      vector <Position>  rectanglesEnd;
   };

   /*********************************************
    * name:    DRAW
    * input:   standard
    * output:  check the retangles
    *********************************************/
   void draw()
   {  // setup
      Position pos;
      Ground g;
      setupStandardFixture(g);
      ogstreamSpy goutSpy;
      // exercise
      g.draw(goutSpy);
      // verify
      assertUnit(goutSpy.targets.size() == 1);
      assertUnit(0.0 <= goutSpy.targets.front().x && goutSpy.targets.front().x < 11000.0);
      assertUnit(0.0 <= goutSpy.targets.front().y && goutSpy.targets.front().y < 11000.0);
      assertUnit(goutSpy.rectanglesBegin.size() == 10);
      assertUnit(goutSpy.rectanglesEnd.size() == 10);
      assertEquals(goutSpy.rectanglesBegin[0].x, 0.0);
      assertEquals(goutSpy.rectanglesBegin[1].x, 1100.0);
      assertEquals(goutSpy.rectanglesBegin[2].x, 2200.0);
      assertEquals(goutSpy.rectanglesBegin[3].x, 3300.0);
      assertEquals(goutSpy.rectanglesBegin[4].x, 4400.0);
      assertEquals(goutSpy.rectanglesBegin[5].x, 5500.0);
      assertEquals(goutSpy.rectanglesBegin[6].x, 6600.0);
      assertEquals(goutSpy.rectanglesBegin[7].x, 7700.0);
      assertEquals(goutSpy.rectanglesBegin[8].x, 8800.0);
      assertEquals(goutSpy.rectanglesBegin[9].x, 9900.0);
      assertEquals(goutSpy.rectanglesBegin[0].y, 0.0);
      assertEquals(goutSpy.rectanglesBegin[1].y, 0.0);
      assertEquals(goutSpy.rectanglesBegin[2].y, 0.0);
      assertEquals(goutSpy.rectanglesBegin[3].y, 0.0);
      assertEquals(goutSpy.rectanglesBegin[4].y, 0.0);
      assertEquals(goutSpy.rectanglesBegin[5].y, 0.0);
      assertEquals(goutSpy.rectanglesBegin[6].y, 0.0);
      assertEquals(goutSpy.rectanglesBegin[7].y, 0.0);
      assertEquals(goutSpy.rectanglesBegin[8].y, 0.0);
      assertEquals(goutSpy.rectanglesBegin[9].y, 0.0);
      assertEquals(goutSpy.rectanglesEnd[0].x, 1100.0);
      assertEquals(goutSpy.rectanglesEnd[1].x, 2200.0);
      assertEquals(goutSpy.rectanglesEnd[2].x, 3300.0);
      assertEquals(goutSpy.rectanglesEnd[3].x, 4400.0);
      assertEquals(goutSpy.rectanglesEnd[4].x, 5500.0);
      assertEquals(goutSpy.rectanglesEnd[5].x, 6600.0);
      assertEquals(goutSpy.rectanglesEnd[6].x, 7700.0);
      assertEquals(goutSpy.rectanglesEnd[7].x, 8800.0);
      assertEquals(goutSpy.rectanglesEnd[8].x, 9900.0);
      assertEquals(goutSpy.rectanglesEnd[9].x, 11000.0);
      assertEquals(goutSpy.rectanglesEnd[0].y, 9900.0);
      assertEquals(goutSpy.rectanglesEnd[1].y, 8800.0);
      assertEquals(goutSpy.rectanglesEnd[2].y, 7700.0);
      assertEquals(goutSpy.rectanglesEnd[3].y, 6600.0);
      assertEquals(goutSpy.rectanglesEnd[4].y, 5500.0);
      assertEquals(goutSpy.rectanglesEnd[5].y, 4400.0);
      assertEquals(goutSpy.rectanglesEnd[6].y, 3300.0);
      assertEquals(goutSpy.rectanglesEnd[7].y, 2200.0);
      assertEquals(goutSpy.rectanglesEnd[8].y, 1100.0);
      assertEquals(goutSpy.rectanglesEnd[9].y, 0.0);
      verifyStandardFixture(g);
      // teardown
      teardownStandardFixture(g);
   }


   /*****************************************************************
    *****************************************************************
    * SETTERS
    *****************************************************************
    *****************************************************************/

   /*********************************************
    * name:    RESET TEN POINTS
    * input:   (3300, 4400)
    * output:  ground...
    *********************************************/
   void reset_ten()
   {  // setup
      Position posHowitzer;
      Ground g;
      setupStandardFixture(g);
      posHowitzer.x = 3300.0;  // 3px
      posHowitzer.y = 4400.0;  // 4px
      // exercise
      g.reset(posHowitzer);
      // verify
      assertUnit(g.iHowitzer == 3);
      assertUnit(g.iTarget >= 0 && g.iTarget < 10);
      assertEquals(g.posUpperRight.x, 11000.0);  // 10px
      assertEquals(g.posUpperRight.y, 11000.0);  // 10px
      assertUnit(g.ground != nullptr);
      if (g.ground != nullptr)
      {
         assertUnit(g.ground[0] >= 0.0 && g.ground[0] < 10.0);
         assertUnit(g.ground[1] >= 0.0 && g.ground[1] < 10.0);
         assertUnit(g.ground[2] >= 0.0 && g.ground[2] < 10.0);
         assertUnit(g.ground[3] >= 0.0 && g.ground[3] < 10.0);
         assertUnit(g.ground[4] >= 0.0 && g.ground[4] < 10.0);
         assertUnit(g.ground[5] >= 0.0 && g.ground[5] < 10.0);
         assertUnit(g.ground[6] >= 0.0 && g.ground[6] < 10.0);
         assertUnit(g.ground[7] >= 0.0 && g.ground[7] < 10.0);
         assertUnit(g.ground[8] >= 0.0 && g.ground[8] < 10.0);
         assertUnit(g.ground[9] >= 0.0 && g.ground[9] < 10.0);
      }
      // teardown
      teardownStandardFixture(g);
   }  



   /*****************************************************************
    *****************************************************************
    * STANDARD FIXTURE
    *****************************************************************
    *****************************************************************/


   // standard fixture: 10 x 10 with howitzer at 5 and target at 7
   void setupStandardFixture(Ground& g)
   {
      metersFromPixels = g.posUpperRight.metersFromPixels;
      g.posUpperRight.metersFromPixels = 1100.0;

      // delete the old
      if (g.ground != nullptr)
         delete [] g.ground;
      g.ground = new double[10];

      for (int i = 0; i < 10; i++)
         g.ground[i] = 9.0 - (double)i;

      g.posUpperRight.x = 11000.0;   // 10 pixels
      g.posUpperRight.y = 11000.0;   // 10 pixels
      g.iHowitzer = 5;
      g.iTarget = 7;
   }

   // verify the standard fixture: 10 x 10 with howitzer at 5 and target at 7
   void verifyStandardFixture(Ground& g)
   {
      assertUnit(g.iHowitzer == 5);
      assertUnit(g.iTarget == 7);
      assertEquals(g.posUpperRight.x, 11000.0);
      assertEquals(g.posUpperRight.y, 11000.0);
      assertEquals(g.posUpperRight.metersFromPixels, 1100.0);
      assert(g.ground != nullptr);
      if (g.ground != nullptr)
      {
         assertEquals(g.ground[0], 9.0);
         assertEquals(g.ground[1], 8.0);
         assertEquals(g.ground[2], 7.0);
         assertEquals(g.ground[3], 6.0);
         assertEquals(g.ground[4], 5.0);
         assertEquals(g.ground[5], 4.0);
         assertEquals(g.ground[6], 3.0);
         assertEquals(g.ground[7], 2.0);
         assertEquals(g.ground[8], 1.0);
         assertEquals(g.ground[9], 0.0);
      }
   }

   // standard fixture: teardown
   void teardownStandardFixture(Ground& g)
   {
      assertUnit(-1.0 != metersFromPixels);
      g.posUpperRight.metersFromPixels = metersFromPixels;
   }
};
