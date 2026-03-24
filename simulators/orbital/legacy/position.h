/***********************************************************************
 * Header File:
 *    Point : The representation of a position 
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Everything we need to know about a location on the screen
 *    or the location on the field.
 ************************************************************************/


#pragma once

#include <iostream> 
#include <cmath>

using namespace std;

class TestPosition;
class Acceleration;
class Velocity;

/*********************************************
 * Position
 * A single position on the field in Meters  
 *********************************************/
class Position
{
public:
   friend TestPosition;
   
   // constructors
   Position()            : x(0.0), y(0.0)  {}
   Position(double x, double y);
   Position(const Position & pt) : x(pt.x), y(pt.y) {}
   Position& operator = (const Position& pt);

   // getters
   double getMetersX()       const { return x;                    }
   double getMetersY()       const { return y;                    }
   double getPixelsX()       const { return x / metersFromPixels; }
   double getPixelsY()       const { return y / metersFromPixels; }

   // setters
   void setMeters(double xMeters, double yMeters) {x = xMeters; y = yMeters; }
   void setMetersX(double xMeters)    { x = xMeters;                         }
   void setMetersY(double yMeters)    { y = yMeters;                         }
   void setPixelsX(double xPixels)    { x = xPixels * metersFromPixels;      }
   void setPixelsY(double yPixels)    { y = yPixels * metersFromPixels;      }
   void addMetersX(double dxMeters)   { setMetersX(getMetersX() + dxMeters); }
   void addMetersY(double dyMeters)   { setMetersY(getMetersY() + dyMeters); }
   void addPixelsX(double dxPixels)   { setPixelsX(getPixelsX() + dxPixels); }
   void addPixelsY(double dyPixels)   { setPixelsY(getPixelsY() + dyPixels); }


   // deal with the ratio of meters to pixels
   void setZoom(double metersFromPixels)
   {
      this->metersFromPixels = metersFromPixels;
   }
   static double getZoom() { return metersFromPixels; }

private:
   double x;                 // horizontal position
   double y;                 // vertical position
   static double metersFromPixels;
};

/*********************************************
 * COMPUTE DISTANCE
 * Find the distance between two positions
 *********************************************/
inline double computeDistance(const Position& pos1, const Position& pos2)
{
   return sqrt(  (pos1.getMetersX() - pos2.getMetersX())
               * (pos1.getMetersX() - pos2.getMetersX())
               + (pos1.getMetersY() - pos2.getMetersY())
               * (pos1.getMetersY() - pos2.getMetersY()));
}

// stream I/O useful for debugging
ostream & operator << (ostream & out, const Position& pt);
istream & operator >> (istream & in,        Position& pt);


/*********************************************
 * PT
 * Trivial point
 *********************************************/
struct PT
{
   double x;
   double y;
};

/************************************************************
 * POSITION : MOCK POSITION
 * Mock class for testing Position functionality
 * Tracks calls to constructors and setters
 * This class is used to test the Position class without
 * requiring the actual Position class to be instantiated.
 ************************************************************/
class MockPosition : public Position
{
public:
   // Constructors
   MockPosition() : Position()
   { defaultConstructorCalls++; }
   
   MockPosition(double x, double y) : Position(x, y)
   { parameterizedConstructorCalls++; }
   
   MockPosition(const Position& pt) : Position(pt)
   { copyConstructorCalls++; }
   
   // Override setters to track calls
   void setMeters(double xMeters, double yMeters)
   {
      Position::setMeters(xMeters, yMeters);
      setMetersCalls++;
   }
   
   void setMetersX(double xMeters)
   {
      Position::setMetersX(xMeters);
      setMetersXCalls++;
   }
   
   void setMetersY(double yMeters)
   {
      Position::setMetersY(yMeters);
      setMetersYCalls++;
   }
   
   void setPixelsX(double xPixels)
   {
      Position::setPixelsX(xPixels);
      setPixelsXCalls++;
   }
   
   void setPixelsY(double yPixels)
   {
      Position::setPixelsY(yPixels);
      setPixelsYCalls++;
   }
   
   void addMetersX(double dxMeters)
   {
      Position::addMetersX(dxMeters);
      addMetersXCalls++;
   }
   
   void addMetersY(double dyMeters)
   {
      Position::addMetersY(dyMeters);
      addMetersYCalls++;
   }
   
   void setZoom(double metersFromPixels)
   {
      Position::setZoom(metersFromPixels);
      setZoomCalls++;
   }
   
   // Test helper methods
   static int getDefaultConstructorCalls() { return defaultConstructorCalls; }
   static int getParameterizedConstructorCalls()
   {
      return parameterizedConstructorCalls;
   }
   static int getCopyConstructorCalls()    { return copyConstructorCalls;    }
   static int getSetMetersCalls()          { return setMetersCalls;          }
   static int getSetMetersXCalls()         { return setMetersXCalls;         }
   static int getSetMetersYCalls()         { return setMetersYCalls;         }
   static int getSetPixelsXCalls()         { return setPixelsXCalls;         }
   static int getSetPixelsYCalls()         { return setPixelsYCalls;         }
   static int getAddMetersXCalls()         { return addMetersXCalls;         }
   static int getAddMetersYCalls()         { return addMetersYCalls;         }
   static int getSetZoomCalls()            { return setZoomCalls;            }
   
   static void reset()
   {
      defaultConstructorCalls = 0;
      parameterizedConstructorCalls = 0;
      copyConstructorCalls = 0;
      setMetersCalls = 0;
      setMetersXCalls = 0;
      setMetersYCalls = 0;
      setPixelsXCalls = 0;
      setPixelsYCalls = 0;
      addMetersXCalls = 0;
      addMetersYCalls = 0;
      setZoomCalls = 0;
   }
   
private:
   static int defaultConstructorCalls;
   static int parameterizedConstructorCalls;
   static int copyConstructorCalls;
   static int setMetersCalls;
   static int setMetersXCalls;
   static int setMetersYCalls;
   static int setPixelsXCalls;
   static int setPixelsYCalls;
   static int addMetersXCalls;
   static int addMetersYCalls;
   static int setZoomCalls;
};
