/**********************************************************************
 * Header File:
 *    SIMULATION
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Execute one simulation of a projectile being fired.
 ************************************************************************/

 /**********************************************************************
  * Header File:
  *    SIMULATION
  * Author:
  *    Taden Marston & Mark Van Horn
  * Summary:
  *    Execute one simulation of a projectile being fired.
  ************************************************************************/

#pragma once
#include "position.h"
#include "ground.h"
#include "howitzer.h"
#include "projectile.h"
#include "uiDraw.h"
#include "uiInteract.h"

class Simulator
{
public:
   Simulator(const Position& posUpperRight);
   void update(double timeStep);
   void handleInput(const Interface* pUI);
   void draw(ogstream& gout) const;
   Position getPosUpperRight() const; // Getter for posUpperRight

private:
   Ground ground;
   Howitzer howitzer;
   Projectile projectile;
	Position posHowitzer;
   Position posUpperRight;
   Position projectilePath[20];
   double angle;
   double time;
   bool isFiring;
   bool isHit;
};
