#include "app/scene_status_formatter.h"

#include <iomanip>
#include <sstream>

#include "simulators/howitzer/howitzer_scene.h"
#include "simulators/lander/lander_scene.h"
#include "simulators/orbital/orbital_scene.h"

namespace
{
std::string formatDouble(double value, int precision = 2)
{
   std::ostringstream stream;
   stream << std::fixed << std::setprecision(precision) << value;
   return stream.str();
}
}

namespace SceneStatusFormatter
{
std::vector<std::string> format(const Application & app)
{
   if (!app.hasActiveScene() || app.activeScene() == nullptr)
      return {};

   if (const auto * lander = dynamic_cast<const LanderScene *>(app.activeScene()))
   {
      return
      {
         "Time: " + formatDouble(lander->elapsedSeconds()) + "s",
         "Altitude: " + formatDouble(lander->demoAltitudeMeters()) + "m",
         "Velocity: " + formatDouble(lander->demoVelocityMetersPerSecond()) + "m/s",
         "Fuel: " + formatDouble(lander->demoFuelPounds()) + "lb"
      };
   }

   if (const auto * howitzer = dynamic_cast<const HowitzerScene *>(app.activeScene()))
   {
      return
      {
         "Time: " + formatDouble(howitzer->elapsedSeconds()) + "s",
         "Angle: " + formatDouble(howitzer->elevationDegrees()) + "deg",
         "Projectile: " + std::string(howitzer->projectileActive() ? "active" : "idle"),
         "Target: " + formatDouble(howitzer->targetDistanceMeters()) + "m"
      };
   }

   if (const auto * orbital = dynamic_cast<const OrbitalScene *>(app.activeScene()))
   {
      return
      {
         "Time: " + formatDouble(orbital->elapsedSeconds()) + "s",
         "Live bodies: " + std::to_string(orbital->liveObjectCount()),
         "Projectiles: " + std::to_string(orbital->projectileCount()),
         "Fragments: " + std::to_string(orbital->fragmentCount())
      };
   }

   return
   {
      "Time: " + formatDouble(app.activeSceneElapsedSeconds()) + "s",
      app.activeSceneStatusText()
   };
}
}
