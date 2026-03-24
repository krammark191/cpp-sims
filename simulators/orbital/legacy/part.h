/***********************************************************************
 * Header File:
 *    PART
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Class to represent a debris part in orbit
 ************************************************************************/

#pragma once

#include "OrbitalObject.h"
#include "fragment.h"
#include <vector>
#include <cmath>

enum class PartType
{
   HUBBLE_COMPUTER,
   HUBBLE_TELESCOPE,
   HUBBLE_LEFT,
	HUBBLE_RIGHT,
   GPS_CENTER,
   GPS_LEFT,
   GPS_RIGHT,
   STARLINK_BODY,
   STARLINK_ARRAY,
   DRAGON_CENTER,
   DRAGON_LEFT,
	DRAGON_RIGHT,
   SPUTNIK_BODY,
   GENERIC // fallback or default part
};

class Part : public OrbitalObject
{
public:
   // Constructors
   Part() : OrbitalObject(), type(PartType::GENERIC)
   {
      radius = getRadiusForType(type);
   }
   Part(const Position& position, const Velocity& velocity)
      : OrbitalObject(position, velocity, Angle()), type(PartType::GENERIC)
   {
      radius = getRadiusForType(type);
   }
   Part(const Position& position, const Velocity& velocity, PartType type)
      : OrbitalObject(position, velocity, Angle()), type(type)
   {
      radius = getRadiusForType(type);
   }
   
   // Type Getter
   PartType getType() const { return type; }
   
   // Type Setter
   void setType(PartType t) { type = t;    }

   // Override pure virtual methods from OrbitalObject
   void update(double time) override;
   void draw(ogstream& gout) override;

   // Override onCollision: when destroyed, spawn fragments
   void onCollision(vector<Part>& parts, vector<Fragment>& fragments) override;

private:
   PartType type;

   static double getRadiusForType(PartType type)
   {
      switch (type)
      {
      case PartType::HUBBLE_COMPUTER:     return 7.0 *  OrbitalObject::ZOOM;
      case PartType::HUBBLE_TELESCOPE:    return 10.0 * OrbitalObject::ZOOM;
      case PartType::HUBBLE_LEFT:         return 8.0 *  OrbitalObject::ZOOM;
      case PartType::HUBBLE_RIGHT:        return 8.0 *  OrbitalObject::ZOOM;
      case PartType::GPS_CENTER:          return 7.0 *  OrbitalObject::ZOOM;
      case PartType::GPS_LEFT:            return 8.0 *  OrbitalObject::ZOOM;
      case PartType::GPS_RIGHT:           return 8.0 *  OrbitalObject::ZOOM;
      case PartType::STARLINK_BODY:       return 2.0 *  OrbitalObject::ZOOM;
      case PartType::STARLINK_ARRAY:      return 4.0 *  OrbitalObject::ZOOM;
      case PartType::DRAGON_CENTER:       return 6.0 *  OrbitalObject::ZOOM;
      case PartType::DRAGON_LEFT:         return 6.0 *  OrbitalObject::ZOOM;
      case PartType::DRAGON_RIGHT:        return 6.0 *  OrbitalObject::ZOOM;
      case PartType::GENERIC: default:    return 3.0 *  OrbitalObject::ZOOM;
      }
   }
};

class DummyPart : public Part
{
public:
   DummyPart() : Part() { }
   DummyPart(const Position& p, const Velocity& v) : Part(p, v) { }
   void draw(ogstream&) override { }
};
