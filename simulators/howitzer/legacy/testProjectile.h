/***********************************************************************
 * Header File:
 *    TEST PROJECTILE
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    All the unit tests for Projectile
 ************************************************************************/


#pragma once

#include "projectile.h"
#include "unitTest.h"


using namespace std;

/*******************************
 * TEST PROJECTILE
 * A friend class for Projectile which contains the Projectile unit tests
 ********************************/
class TestProjectile : public UnitTest
{
public:
   void run()
   {
      // Ticket 3: Setup
      defaultConstructor();
      reset_empty();
      reset_full();
      fire_right();
      fire_left();
      fire_up();
      
      // Ticket 4: Advance
      advance_nothing();
      advance_fall();
      advance_horizontal();
      advance_up();
      advance_diagonalUp();
      advance_diagonalDown();
      
      report("Projectile");
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
    * output:  mass=46.7, radius=0.077545 flightPath={}
    *********************************************/
   void defaultConstructor()
   {  // setup
      Projectile p;
      // exercise
      // verify
      assertUnit(p.mass == 46.7);
      assertUnit(p.radius == 0.077545);
      assertUnit(p.flightPath.empty());
   }
   
   /*********************************************
    * name:    RESET from empty
    * input:   mass=-99, radius=-99, flightPath={}
    * output:  mass=46.7, radius=0.077545 flightPath={}
    *********************************************/
   void reset_empty()
   {  //setup
      Projectile p;
      p.mass = -99;
      p.radius = -99;
      p.flightPath = {};
      
      //exercise
      p.reset();
      
      //verify
      assertUnit(p.mass == 46.7);
      assertUnit(p.radius == 0.077545);
      assertUnit(p.flightPath.empty());
      //teardown
   }
   
   /*********************************************
    * name:    RESET with a non-zero flight path
    * input:   mass=-99, radius=-99, flightPath={0,1,2}
    * output:  mass=46.7, radius=0.077545 flightPath={}
    *********************************************/
   void reset_full()
   {  // setup
      Projectile p;
      Projectile::PositionVelocityTime pvt;
      p.mass = -99;
      p.radius = -99;
      p.flightPath.push_back(pvt);
      p.flightPath.push_back(pvt);
      p.flightPath.push_back(pvt);
      
      //exercise
      p.reset();
      
      //verify
      assertUnit(p.mass == 46.7);
      assertUnit(p.radius = 0.077545);
      assertUnit(p.flightPath.empty());
   }  // teardown
   
   /*********************************************
    * name:    FIRE horizontally right
    * input:   angle=90  pos=(111,222) muzzleVelocity=100
    * output:  flightPath={pos=111,222 v=100,0 t=1}
    *********************************************/
   void fire_right()
   {  // setup
      Projectile p;
      Angle a(90.0);
      Position pos(111.0, 222.0);
      double muzzleVelocity = 100;
      
      // exercise
      p.fire(pos, a, muzzleVelocity, 1);
      
      // verify
      assertUnit(p.flightPath.size() == 1);
      if (!p.flightPath.empty())
      {
         Projectile::PositionVelocityTime pvt = p.flightPath.front();
         assertUnit(pvt.pos.x == 111.0);
         assertUnit(pvt.pos.y == 222.0);
         assertEquals(pvt.v.dx, 100.0);
         assertEquals(pvt.v.dy, 0.0);
         assertUnit(pvt.t == 1.0);
      }
   }  // teardown
   
   /*********************************************
    * name:    FIRE horizontally left
    * input:   angle=-90  pos=(111,222) muzzleVelocity=100
    * output:  flightPath={pos=111,222 v=-100,0 t=1}
    *********************************************/
   void fire_left()
   {  // setup
      Projectile p;
      Angle a(-90.0);
      Position pos(111.0, 222.0);
      double muzzleVelocity = 100;
      
      // exercise
      p.fire(pos, a, muzzleVelocity, 1);
      
      // verify
      assertUnit(p.flightPath.size() == 1);
      if (!p.flightPath.empty())
      {
         Projectile::PositionVelocityTime pvt = p.flightPath.front();
         assertUnit(pvt.pos.x == 111.0);
         assertUnit(pvt.pos.y == 222.0);
         assertEquals(pvt.v.dx, -100.0);
         assertEquals(pvt.v.dy, 0.0);
         assertUnit(pvt.t == 1.0);
      }
   }  // teardown
   
   /*********************************************
    * name:    FIRE straight up
    * input:   angle=0  pos=(111,222) muzzleVelocity=100
    * output:  flightPath={pos=111,222 v=0,100 t=1}
    *********************************************/
   void fire_up()
   {  // setup
      Projectile p;
      Angle a(0.0);
      Position pos(111.0, 222.0);
      double muzzleVelocity = 100;
      
      // exercise
      p.fire(pos, a, muzzleVelocity, 1);
      
      // verify
      assertUnit(p.flightPath.size() == 1);
      if (!p.flightPath.empty())
      {
         Projectile::PositionVelocityTime pvt = p.flightPath.front();
         assertUnit(pvt.pos.x == 111.0);
         assertUnit(pvt.pos.y == 222.0);
         assertUnit(pvt.v.dx == 0.0);
         assertUnit(pvt.v.dy == 100.0);
         assertUnit(pvt.t == 1.0);
      }
   }  // teardown
   
   /*****************************************************************
    *****************************************************************
    * ADVANCE
    *****************************************************************
    *****************************************************************/
   
   /*********************************************
    * name:    ADVANCE : the projectile is not fired. Nothing will happen
    * input:   flightPath={}
    * output:  flightPath={}
    *********************************************/
   void advance_nothing()
   {
      // setup
      Projectile p;
      // exercise
      p.advance(0);
      // verify
      assertUnit(p.flightPath.empty());
   }
   
   /*********************************************
    * name:    ADVANCE : the projectile is stationary and falls down
    * input:   flightPath={pos=100,200 v=0,0 t=100}
    * output:  flightPath={}{pos.x=100      = 100 + 0*1
    *                        pos.y=195.0968 = 200 + 0*1 + .5(-9.806)*1*1
    *                        v.dx =0        = 0 + 0*1
    *                        v.dy =-9.8     = 0 + (-9.8064)*1
    *                        t=101}
    *********************************************/
   void advance_fall()
   {
      // setup
      Projectile p;
      Projectile::PositionVelocityTime pvt;
      pvt.pos.x = 100.0;
      pvt.pos.y = 200.0;
      pvt.v.dx = 0.0;
      pvt.v.dy = 0.0;
      pvt.t = 100.0;
      p.flightPath.push_back(pvt);
      // exercise
      p.advance(101.0);
      // verify
      assertUnit(p.flightPath.size() == 2);
      assertEquals(p.flightPath.back().pos.x, 100.0);      // 100 + 0*1
      assertEquals(p.flightPath.back().pos.y, 195.0968);   // 200 + 0*1 + .5(-9.806)*1*1
      assertEquals(p.flightPath.back().v.dx, 0.0);         // 0 + 0*1
      assertEquals(p.flightPath.back().v.dy, -9.8064);     // 0 + (-9.8064)*1
      assertEquals(p.flightPath.back().t, 101.0);
   }
   
   /*********************************************
    * name:    ADVANCE : the projectile is traveling horizontally
    * input:   flightPath={pos=100,200 v=50,0 t=100}
    * output:  flightPath={}{pos.x=149.9756 = 100 + 50*1 + .5(-0.0487)*1*1
    *                        pos.y=195.0968 = 200 + 0*1  + .5(-9.8064)*1*1
    *                        v.dx =49.9513  = 50 + (-0.0487)*1
    *                        v.dy =-9.8064  = 0  + (-9.8064)*1
    *                        t=101}
    *********************************************/
   void advance_horizontal()
   {
      // setup
      Projectile p;
      Projectile::PositionVelocityTime pvt;
      pvt.pos.x = 100.0;
      pvt.pos.y = 200.0;
      pvt.v.dx = 50.0;
      pvt.v.dy = 0.0;
      pvt.t = 100.0;
      p.flightPath.push_back(pvt);
      // exercise
      p.advance(101.0);
      // verify
      assertUnit(p.flightPath.size() == 2);
      assertEquals(p.flightPath.back().pos.x, 149.9756);   // 100 + 50*1 + .5(-0.0487)*1*1
      assertEquals(p.flightPath.back().pos.y, 195.0968);   // 200 + 0*1 + .5(-9.8064)*1*1
      assertEquals(p.flightPath.back().v.dx, 49.9513);     // 50 + (-0.0487)*1
      assertEquals(p.flightPath.back().v.dy, -9.8064);     // 0 + (-9.8064)*1
      assertEquals(p.flightPath.back().t, 101.0);
   }
   
   /*********************************************
    * name:    ADVANCE : the projectile is traveling up, no horizontal position change
    * input:   flightPath={pos=100,200 v=0,100 t=100}
    * output:  flightPath={}{pos.x=100      = 0   + 0*1   + .5(0)*1*1
    *                        pos.y=294.9021 = 200 + 100*1 + .5(-9.8064-.3893)*1*1
    *                        v.dx =0        = 0   + 0*1
    *                        v.dy =89.8042  = 100 + (-9.8064-.3893)
    *                        t=101}
    *********************************************/
   void advance_up()
   {
      // setup
      Projectile p;
      Projectile::PositionVelocityTime pvt;
      pvt.pos.x = 100.0;
      pvt.pos.y = 200.0;
      pvt.v.dx = 0.0;
      pvt.v.dy = 100.0;
      pvt.t = 100.0;
      p.flightPath.push_back(pvt);
      // exercise
      p.advance(101.0);
      // verify
      assertUnit(p.flightPath.size() == 2);
      assertEquals(p.flightPath.back().pos.x, 100.0);      // 0 + 0*1 + .5(0)*1*1
      assertEquals(p.flightPath.back().pos.y, 294.9021);   // 200 + 100*1 + .5(-9.8064-.3893)*1*1
      assertEquals(p.flightPath.back().v.dx, 0.0);         // 0 + 0*1
      assertEquals(p.flightPath.back().v.dy, 89.8042);     // 100 + (-9.8064-.3893)
      assertEquals(p.flightPath.back().t, 101.0);
   }
   
   /*********************************************
    * name:    ADVANCE : the projectile is traveling up and across
    * input:   flightPath={pos=100,200 v=50,40 t=100}
    * output:  flightPath={}{pos.x=149.9600 = 100 + 50*1 + .5(-0.0799)*1*1
    *                        pos.y=235.0648 = 200 + 40*1 + .5(-9.8064-0.0638)*1*1
    *                        v.dx =49.9201  = 50 + (-.0799)*1
    *                        v.dy =30.1297  = 40 + (-9.8064-0.0638)*1
    *                        t=101}
    *********************************************/
   void advance_diagonalUp()
   {
      // setup
      Projectile p;
      Projectile::PositionVelocityTime pvt;
      pvt.pos.x = 100.0;
      pvt.pos.y = 200.0;
      pvt.v.dx = 50.0;
      pvt.v.dy = 40.0;
      pvt.t = 100.0;
      p.flightPath.push_back(pvt);
      // exercise
      p.advance(101.0);
      // verify
      assertUnit(p.flightPath.size() == 2);
      assertEquals(p.flightPath.back().pos.x, 149.9600);   // 100 + 50*1 + .5(-0.0799)*1*1
      assertEquals(p.flightPath.back().pos.y, 235.0648);   // 200 + 40*1 + .5(-9.8064-0.0638)*1*1
      assertEquals(p.flightPath.back().v.dx, 49.9201);     // 50 + (-.0799)*1
      assertEquals(p.flightPath.back().v.dy, 30.1297);     // 40 + (-9.8064-0.0638)*1
      assertEquals(p.flightPath.back().t, 101.0);
   }
   
   /*********************************************
    * name:    ADVANCE : the projectile is traveling down and across
    * input:   flightPath={}{}{pos=100,200 v=50,-40 t=100}
    * output:  flightPath={}{}{}{pos.x=149.9201 = 100 + 50*1 + .5(-0.0799)*1*1
    *                            pos.y=155.1287 = 200 +-40*1 + .5(-9.8064+0.0638)*1*1
    *                            v.dx = 49.9201 =  50 + (-0.0799)*1
    *                            v.dy =-49.7425 = -40 + (-9.8064+0.0638)*1*1
    *                            t=101}
    *********************************************/
   void advance_diagonalDown()
   {  // setup
      setupStandardFixture();
      Position pos;
      Projectile p;
      Projectile::PositionVelocityTime pvt;
      p.flightPath.push_back(pvt);
      p.flightPath.push_back(pvt);
      pvt.pos.x = 100.0;
      pvt.pos.y = 200.0;
      pvt.v.dx = 50.0;
      pvt.v.dy = -40.0;
      pvt.t = 100.0;
      p.flightPath.push_back(pvt);
      // exercise
      p.advance(101.0);
      // verify
      assertUnit(p.flightPath.size() == 4);
      assertEquals(p.mass, 46.7);
      assertEquals(p.radius, 0.077545);
      assertUnit(!p.flightPath.empty());
      if (!p.flightPath.empty())
      {
         assertEquals(p.flightPath.back().pos.x, 149.9601); // 100 + 50*1 + .5(-0.0799)*1*1
         assertEquals(p.flightPath.back().pos.y, 155.1287); // 200 +-40*1 + .5(-9.8064+0.0638)*1*1
         assertEquals(p.flightPath.back().v.dx, 49.9201);   // 50 + (-0.0799)*1
         assertEquals(p.flightPath.back().v.dy, -49.7425);  //-40 + (-9.8064+0.0638)*1*1
         assertEquals(p.flightPath.back().t, 101.0);
      }
      // teardown
      teardownStandardFixture();
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
      p.metersFromPixels = 1.0;
   }
   
   // teardown the standard fixture - reset the zoom to what it was previously
   void teardownStandardFixture()
   {
      assert(metersFromPixels != -1.0);
      Position p;
      p.metersFromPixels = metersFromPixels;
   }
   
};

