/***********************************************************************
 * Header File:
 *    TEST PART
 * Author:
 *    Mark Van Horn & Taden Marston
 * Summary:
 *    Unit tests for the Part class.
 ************************************************************************/

#pragma once
#include "unitTest.h"
#include "part.h"

/*********************************
 * TEST PART
 * A friend class for Part which contains the unit tests
 *********************************/
class TestPart : public UnitTest
{
public:
   void run()
   {
      test_defaultConstructor();
      test_nonDefaultConstructorPosition();
      test_nonDefaultConstructorWithType();
      test_getRadius();
      test_getType();
      test_setType();
      test_onCollision();
      
      report("Part");
   }
   
private:
   void test_defaultConstructor()
   {
      // setup
      DummyPart part;
      Position positionDefault;
      Velocity velocityDefault;
      Angle angleDefault;
      
      // exercise
      Position positionPart = part.getPosition();
      Velocity velocityPart = part.getVelocity();
      Angle anglePart = part.getAngleShip();
      PartType typePart = part.getType();
      
      // verify
      assertEquals(positionPart.getMetersX(), positionDefault.getMetersX());
      assertEquals(positionPart.getMetersY(), positionDefault.getMetersY());
      assertEquals(velocityPart.getDX(), velocityDefault.getDX());
      assertEquals(velocityPart.getDY(), velocityDefault.getDY());
      assertEquals(anglePart.getDx(), angleDefault.getDx());
      assertEquals(anglePart.getDy(), angleDefault.getDy());
      assertEquals(typePart == PartType::GENERIC, true);
      assertEquals(part.getRadius(), 3.0 * OrbitalObject::ZOOM);
   }  // cleanup
   
   void test_nonDefaultConstructorPosition()
   {
      // setup
      Position pos(100.0, 200.0);
      Velocity vel(5.0, -3.0);
      
      // exercise
      DummyPart part(pos, vel);
      
      // verify
      assertEquals(part.getPosition().getMetersX(), pos.getMetersX());
      assertEquals(part.getPosition().getMetersY(), pos.getMetersY());
      assertEquals(part.getVelocity().getDX(), vel.getDX());
      assertEquals(part.getVelocity().getDY(), vel.getDY());
      assertEquals(part.getType() == PartType::GENERIC, true);
      assertEquals(part.getRadius(), 3.0 * OrbitalObject::ZOOM);
   }  // cleanup
   
   void test_nonDefaultConstructorWithType()
   {
      // setup
      Position pos(100.0, 200.0);
      Velocity vel(5.0, -3.0);
      PartType type = PartType::HUBBLE_TELESCOPE;
      
      // exercise
      Part part(pos, vel, type);
      
      // verify
      assertEquals(part.getPosition().getMetersX(), pos.getMetersX());
      assertEquals(part.getPosition().getMetersY(), pos.getMetersY());
      assertEquals(part.getVelocity().getDX(), vel.getDX());
      assertEquals(part.getVelocity().getDY(), vel.getDY());
      assertEquals(part.getType() == PartType::HUBBLE_TELESCOPE, true);
      assertEquals(part.getRadius(), 10.0 * OrbitalObject::ZOOM);
   }  // cleanup
   
   void test_getRadius()
   {
      // setup
      Part hubblePart(Position(), Velocity(), PartType::HUBBLE_COMPUTER);
      Part gpsPart(Position(), Velocity(), PartType::GPS_CENTER);
      Part starlinkPart(Position(), Velocity(), PartType::STARLINK_BODY);
      
      // verify
      assertEquals(hubblePart.getRadius(), 7.0 * OrbitalObject::ZOOM);
      assertEquals(gpsPart.getRadius(), 7.0 * OrbitalObject::ZOOM);
      assertEquals(starlinkPart.getRadius(), 2.0 * OrbitalObject::ZOOM);
   }  // cleanup
   
   void test_getType()
   {
      // setup
      Part part(Position(), Velocity(), PartType::DRAGON_CENTER);
      
      // exercise
      PartType type = part.getType();
      
      // verify
      assertEquals(type == PartType::DRAGON_CENTER, true);
   }  // cleanup
   
   void test_setType()
   {
      // setup
      Part part;
      
      // exercise
      part.setType(PartType::SPUTNIK_BODY);
      
      // verify
      assertEquals(part.getType() == PartType::SPUTNIK_BODY, true);
   }  // cleanup
   
   void test_onCollision()
   {
      // setup
      Part part;
      vector<Part> parts;
      vector<Fragment> fragments;
      size_t initialFragments = fragments.size();
      
      // exercise
      part.onCollision(parts, fragments);
      
      // verify
      assertEquals(part.isAlive(), false);
      assertEquals(fragments.size() > initialFragments, true);
   }  // cleanup
};
