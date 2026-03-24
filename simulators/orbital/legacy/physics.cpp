/***********************************************************************
 * Source File:
 *    PHYSICS
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Everything we need to know about fezziks
 ************************************************************************/

#include "physics.h"

 // Calculate Earth's rotational speed
double calculateEarthSpeed()
{
   return (2.0 * M_PI / 30.0) * (1440.0 / 86400.0); // radians per second
}

// Define the global constant
const double EARTH_SPEED = calculateEarthSpeed();

double getTime()
{
   double frameRate = 30.0;                        // Frames per second
   double hoursPerDay = 24.0;
   double minutesPerHour = 60.0;
   double dilation = hoursPerDay * minutesPerHour; // 1440 minutes per day
   return dilation / frameRate;                    // 48 seconds per frame
}

double getAltitude(const Position& pos)
{
   Position posCenter(0.0, 0.0); // Center of the Earth at (0, 0)
   double distance = computeDistance(posCenter, pos);
   return distance - EARTH_RADIUS;
}

Acceleration getGravity(const Position& pos)
{
   double altitude = getAltitude(pos);
   Angle direction;
   direction.setDxDy(-pos.getMetersX(), -pos.getMetersY());

   double g = 9.80665; // gravitational constant in m/s^2
   double R = EARTH_RADIUS; // radius of the Earth in meters
   double h = altitude; // altitude in meters
   double accelerationMagnitude = g * (R / (R + h)) * (R / (R + h));

   return Acceleration(accelerationMagnitude, direction);
}

Velocity& updateVelocity(Velocity& velocity,
   const Acceleration& acceleration,
   double time)
{
   velocity.addDX(acceleration.getDDX() * time);
   velocity.addDY(acceleration.getDDY() * time);
   return velocity;
}

Position& updatePosition(Position& position,
   const Velocity& velocity,
   const Acceleration& acceleration,
   double time)
{
   position.addMetersX(velocity.getDX()
      * time + 0.5 * acceleration.getDDX() * time * time);
   position.addMetersY(velocity.getDY()
      * time + 0.5 * acceleration.getDDY() * time * time);
   return position;
}

bool detectCollision(const Position& pos1, const Position& pos2,
                     double radius1, double radius2)
{
   double dx = pos2.getMetersX() - pos1.getMetersX();
   double dy = pos2.getMetersY() - pos1.getMetersY();
   double distance = sqrt(dx * dx + dy * dy);
   return distance < (radius1 + radius2);
}
