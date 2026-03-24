/***********************************************************************
 * Source File:
 *    Point : The representation of a position on the screen
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Everything we need to know about a location on the screen.
 ************************************************************************/

#include "position.h"
#include <cassert>


Position::Position(double x, double y)
{
   setMetersX(x);
   setMetersY(y);
}

/******************************************
 * POINT : ASSIGNMENT
 * Assign a point
 *****************************************/
Position& Position::operator = (const Position& pt)
{
   x = pt.x;
   y = pt.y;
   return *this;
}

/******************************************
 * POSITION insertion
 *       Display coordinates on the screen
 *****************************************/
ostream& operator << (ostream& out, const Position& pt)
{
   out << "(" << pt.getMetersX() << "m , " << pt.getMetersY() << "m)";
   return out;
}
   
/*******************************************
* POSITION extraction
*       Prompt for coordinates
******************************************/
istream& operator >> (istream& in, Position& pt)
{
   double x;
   double y;
   in >> x >> y;

   pt.setMetersX(x);
   pt.setMetersY(y);

   return in;
}

double Position::metersFromPixels = 40.0;

int MockPosition::defaultConstructorCalls = 0;
int MockPosition::parameterizedConstructorCalls = 0;
int MockPosition::copyConstructorCalls = 0;
int MockPosition::setMetersCalls = 0;
int MockPosition::setMetersXCalls = 0;
int MockPosition::setMetersYCalls = 0;
int MockPosition::setPixelsXCalls = 0;
int MockPosition::setPixelsYCalls = 0;
int MockPosition::addMetersXCalls = 0;
int MockPosition::addMetersYCalls = 0;
int MockPosition::setZoomCalls = 0;
