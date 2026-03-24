/***********************************************************************
 * Header File:
 *    Dream Chaser
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Everything related to the Dream Chaser spaceplane
 ************************************************************************/

#pragma once

#include "OrbitalObject.h" // Base class for orbital objects
#include "angle.h"         // For handling rotation
#include "velocity.h"      // For handling velocity
#include "acceleration.h"  // For handling acceleration
#include "position.h"      // For handling position
#include "uiDraw.h"        // For drawShip()
#include "physics.h"       // For getTime() and physics calculations
#include "projectile.h"    // For launching projectiles
#include "part.h"
#include "fragment.h"
#include <vector>
#include <cmath>

 /*********************************************
  * DREAM CHASER
  * Represents the user-controlled Dream Chaser spaceplane.
  *********************************************/
class DreamChaser : public OrbitalObject
{
public:
   // Constructors
   DreamChaser();
   DreamChaser(const Position& position, const Velocity& velocity);

   void setThrusting(bool value) { isThrusting = value; }
   bool getThrusting() const { return isThrusting; }

   // User input handling
   virtual void rotateLeft();
   virtual void rotateRight();
   virtual void thrust(double time);
   Projectile launchProjectile();

   // Override virtual methods from OrbitalObject
   virtual void update(double time) override;
   void draw(ogstream& gout) override;

   // Collision handling (spawns parts, then dies)
   void onCollision(vector<Part>& parts, vector<Fragment>& fragments) override;

private:
   // Constants for Dream Chaser movement
   const double ROTATION_AMOUNT = 0.1; // radians
   const double THRUST_MAGNITUDE = 2.0; // m/s^2
   const double PROJECTILE_OFFSET = 19.0; // pixels in front of the ship
   const double PROJECTILE_SPEED_BOOST = 9000.0; // m/s
   bool isThrusting = false; // Flag to indicate if the ship is thrusting
};

class DummyDreamChaser : public DreamChaser
{
public:
   // Track calls
   int rotateLeftCalls = 0;
   int rotateRightCalls = 0;
   int thrustCalls = 0;
   int updateCalls = 0;

   void rotateLeft() override
   {
      rotateLeftCalls++; DreamChaser::rotateLeft();
   }
   void rotateRight() override
   {
      rotateRightCalls++; DreamChaser::rotateRight();
   }
   void thrust(double time) override
   {
      thrustCalls++; DreamChaser::thrust(time);
   }
   void update(double time) override
   {
      updateCalls++; DreamChaser::update(time);
   }

   void onCollision(vector<Part>& parts, vector<Fragment>& fragments) override
   {
      DreamChaser::onCollision(parts, fragments);
   }

   // Add reset method for convenience
   void reset()
   {
      rotateLeftCalls = rotateRightCalls = thrustCalls = updateCalls = 0;
   }
};
