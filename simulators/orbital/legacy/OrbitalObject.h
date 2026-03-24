/***********************************************************************
 * Header File:
 *    Orbital Object
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Everything that orbits around a planet
 ************************************************************************/

#pragma once

#include "position.h"
#include "velocity.h"
#include "angle.h"
#include "uiDraw.h"
#include "physics.h" // Needed for update logic
#include <vector>

using namespace std;

class Part;      // Forward declare to avoid circular include
class Fragment;  // Forward declare

/*********************************************
 * ORBITAL OBJECT
 * The base class for all orbiting entities
 *********************************************/
class OrbitalObject
{
protected:
   Position position;
   Velocity velocity;
   Angle angleShip;
   double angularVelocity;
   double radius;
   bool alive;
   
public:
   static constexpr double ZOOM = 128000.0;
   // Constructors
   OrbitalObject() : position(), velocity(), angleShip(), angularVelocity(0.0),
                     alive(true) { }
   OrbitalObject(const Position& position,
                 const Velocity& velocity, const Angle& angle) :
   position(position),
   velocity(velocity),
   angleShip(angle), angularVelocity(0.0), alive(true) { }
   
   // Getters
   Position getPosition()        const { return position;               }
   Velocity getVelocity()        const { return velocity;               }
   double getHeight()            const { return getAltitude(position);  }
   double getAngleShip()         const { return angleShip.getDegrees(); }
   double getAngularVelocity()   const { return angularVelocity;        }
   double getRadius()            const { return radius;                 }
   bool isAlive()                const { return alive;                  }
   
   // Setters
   void setPosition(const Position& position)   { this->position = position;  }
   void setVelocity(const Velocity& velocity)   { this->velocity = velocity;  }
   void setAngleShip(double degrees)   { this->angleShip.setDegrees(degrees); }
   void setAngularVelocity(double angularVelocity)
   {
      this->angularVelocity = angularVelocity;
   }
   void kill() { alive = false; }
   
   // Virtual methods to be implemented by derived classes
   virtual void update(double time) = 0;
   virtual void draw(ogstream& gout) = 0;
   
   // NEW: Called when this object collides with something deadly.
   // Default is to do nothing.
   virtual void onCollision(vector<Part>&, vector<Fragment>&) {}
};

// ===============================
// TEST DOUBLES (for unit testing)
// ===============================
#ifdef ORBITALOBJECT_ENABLE_TEST_DOUBLES
class DummyOrbitalObject : public OrbitalObject
{
public:
   DummyOrbitalObject() : OrbitalObject() { }
   DummyOrbitalObject(const Position& p, const Velocity& v, const Angle& a) :
   OrbitalObject(p, v, a) { }
   void update(double /*time*/) override { }
   void draw(ogstream& /*gout*/) override { }
   void onCollision(vector<Part>&, vector<Fragment>&) override { kill(); }
};
#endif
