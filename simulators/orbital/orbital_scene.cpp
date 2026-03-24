#include "simulators/orbital/orbital_scene.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "simulators/common/scene_render_utils.h"
#include "simulators/orbital/orbital_legacy_bridge.h"

namespace
{
constexpr double kWorldExtentMeters = 46000000.0;
constexpr double kEarthRenderRadius = 22.0;
constexpr double kHudLeft = 24.0;
constexpr double kHudTop = 680.0;
constexpr double kOrbitPanelWidth = 280.0;
constexpr double kMapMargin = 54.0;

Position worldToScreen(const OrbitalPosition & world, const Position & viewport)
{
   const Position mapBottomLeft(kMapMargin, kMapMargin);
   const Position mapTopRight(viewport.getX() - kOrbitPanelWidth - 28.0, viewport.getY() - 28.0);
   const double mapWidth = mapTopRight.getX() - mapBottomLeft.getX();
   const double mapHeight = mapTopRight.getY() - mapBottomLeft.getY();
   const double centerX = (mapBottomLeft.getX() + mapTopRight.getX()) * 0.5;
   const double centerY = (mapBottomLeft.getY() + mapTopRight.getY()) * 0.5;
   const double scale = (std::min(mapWidth, mapHeight) * 0.44) / kWorldExtentMeters;

   return Position(centerX + world.getMetersX() * scale,
                   centerY + world.getMetersY() * scale);
}

OrbitalPosition worldToLegacyScreen(const OrbitalPosition & world, const Position & viewport)
{
   const Position screen = worldToScreen(world, viewport);
   OrbitalPosition mapped;
   mapped.setPixelsX(screen.getX());
   mapped.setPixelsY(screen.getY());
   return mapped;
}

struct BodyVisual
{
   const OrbitalLegacyObject * object;
   const char * label;
};

struct BackgroundStar
{
   Position position;
   unsigned char phase = 0;
};
}

struct OrbitalScene::Impl
{
   bool active = false;
   double secondsSinceEnter = 0.0;
   InputState input;
   OrbitalPosition viewportMeters;
   double angleEarth = 0.0;
   OrbitalLegacyGPS gps1;
   OrbitalLegacyGPS gps2;
   OrbitalLegacyGPS gps3;
   OrbitalLegacyGPS gps4;
   OrbitalLegacyGPS gps5;
   OrbitalLegacyGPS gps6;
   OrbitalLegacySputnik sputnik;
   OrbitalLegacyHubble hubble;
   OrbitalLegacyDragon dragon;
   OrbitalLegacyStarlink starlink;
   std::unique_ptr<OrbitalLegacyDreamChaser> dreamChaser;
   std::vector<OrbitalLegacyObject *> objects;
   std::vector<OrbitalLegacyProjectile> projectiles;
   std::vector<OrbitalLegacyPart> parts;
   std::vector<OrbitalLegacyFragment> fragments;
   std::vector<BackgroundStar> backgroundStars;
   std::size_t selectedIndex = 0;
   std::string status = "Orbital simulation ready.";

   Impl() :
      viewportMeters(),
      gps1(OrbitalPosition(0.0, 26560000.0), OrbitalVelocity(-3880.0, 0.0)),
      gps2(OrbitalPosition(23001634.72, 13280000.0), OrbitalVelocity(-1940.0, 3360.18)),
      gps3(OrbitalPosition(23001634.72, -13280000.0), OrbitalVelocity(1940.0, 3360.18)),
      gps4(OrbitalPosition(0.0, -26560000.0), OrbitalVelocity(3880.0, 0.0)),
      gps5(OrbitalPosition(-23001634.72, -13280000.0), OrbitalVelocity(1940.0, -3360.18)),
      gps6(OrbitalPosition(-23001634.72, 13280000.0), OrbitalVelocity(-1940.0, -3360.18)),
      sputnik(OrbitalPosition(-36515095.13, 21082000.0), OrbitalVelocity(2050.0, 2684.68)),
      hubble(OrbitalPosition(0.0, -42164000.0), OrbitalVelocity(3100.0, 0.0)),
      dragon(OrbitalPosition(0.0, 8000000.0), OrbitalVelocity(-7900.0, 0.0)),
      starlink(OrbitalPosition(0.0, -13020000.0), OrbitalVelocity(5800.0, 0.0)),
      dreamChaser(std::make_unique<OrbitalLegacyDreamChaser>(
         OrbitalPosition(-57600000.0, 57600000.0), OrbitalVelocity(0.0, -2000.0)))
   {
      viewportMeters.setZoom(128000.0);
      viewportMeters.setPixelsX(1000.0);
      viewportMeters.setPixelsY(1000.0);
      resetWorld();
   }

   void rebuildObjectList()
   {
      objects =
      {
         &gps1, &gps2, &gps3, &gps4, &gps5, &gps6,
         &sputnik, &hubble, &dragon, &starlink, dreamChaser.get()
      };
   }

   void seedBackgroundStars()
   {
      backgroundStars.clear();
      const int count = static_cast<int>(viewportMeters.getPixelsX() / 8.0);
      backgroundStars.reserve(count);
      for (int i = 0; i < count; ++i)
      {
         BackgroundStar star;
         star.position = Position(static_cast<double>(random(0, static_cast<int>(viewportMeters.getPixelsX()))),
                                  static_cast<double>(random(0, static_cast<int>(viewportMeters.getPixelsY()))));
         star.phase = static_cast<unsigned char>(random(0, 255));
         backgroundStars.push_back(star);
      }
   }

   void resetWorld()
   {
      secondsSinceEnter = 0.0;
      angleEarth = 0.0;
      input = InputState{};
      gps1 = OrbitalLegacyGPS(OrbitalPosition(0.0, 26560000.0), OrbitalVelocity(-3880.0, 0.0));
      gps2 = OrbitalLegacyGPS(OrbitalPosition(23001634.72, 13280000.0), OrbitalVelocity(-1940.0, 3360.18));
      gps3 = OrbitalLegacyGPS(OrbitalPosition(23001634.72, -13280000.0), OrbitalVelocity(1940.0, 3360.18));
      gps4 = OrbitalLegacyGPS(OrbitalPosition(0.0, -26560000.0), OrbitalVelocity(3880.0, 0.0));
      gps5 = OrbitalLegacyGPS(OrbitalPosition(-23001634.72, -13280000.0), OrbitalVelocity(1940.0, -3360.18));
      gps6 = OrbitalLegacyGPS(OrbitalPosition(-23001634.72, 13280000.0), OrbitalVelocity(-1940.0, -3360.18));
      sputnik = OrbitalLegacySputnik(OrbitalPosition(-36515095.13, 21082000.0), OrbitalVelocity(2050.0, 2684.68));
      hubble = OrbitalLegacyHubble(OrbitalPosition(0.0, -42164000.0), OrbitalVelocity(3100.0, 0.0));
      dragon = OrbitalLegacyDragon(OrbitalPosition(0.0, 8000000.0), OrbitalVelocity(-7900.0, 0.0));
      starlink = OrbitalLegacyStarlink(OrbitalPosition(0.0, -13020000.0), OrbitalVelocity(5800.0, 0.0));
      dreamChaser = std::make_unique<OrbitalLegacyDreamChaser>(
         OrbitalPosition(-57600000.0, 57600000.0), OrbitalVelocity(0.0, -2000.0));
      gps1.setAngleShip(90.0);
      gps2.setAngleShip(150.0);
      gps3.setAngleShip(210.0);
      gps4.setAngleShip(270.0);
      gps5.setAngleShip(330.0);
      gps6.setAngleShip(30.0);
      rebuildObjectList();
      projectiles.clear();
      parts.clear();
      fragments.clear();
      seedBackgroundStars();
      selectedIndex = 0;
      status = "Orbital simulation ready.";
   }

   std::size_t countLiveBodies() const
   {
      std::size_t count = 0;
      for (const auto * object : objects)
         if (object->isAlive())
            ++count;
      return count;
   }
};

OrbitalScene::OrbitalScene() :
   bindings(
   {
      {"Rotate left", "Left Arrow", "A"},
      {"Rotate right", "Right Arrow", "D"},
      {"Main thrust", "Down Arrow", "W"},
      {"Fire projectile", "Space", "F"},
      {"Pause menu", "Esc", nullptr}
   }),
   impl(std::make_unique<Impl>())
{
}

OrbitalScene::~OrbitalScene() = default;

SimulatorId OrbitalScene::id() const
{
   return SimulatorId::Orbital;
}

const char * OrbitalScene::sceneName() const
{
   return "OrbitalScene";
}

const char * OrbitalScene::statusText() const
{
   return impl->status.c_str();
}

const std::vector<ControlBinding> & OrbitalScene::keybindings() const
{
   return bindings;
}

bool OrbitalScene::supportsRecordedPreview() const
{
   return true;
}

bool OrbitalScene::supportsFutureAiModes() const
{
   return false;
}

bool OrbitalScene::isReady() const
{
   return true;
}

bool OrbitalScene::isPlaceholder() const
{
   return false;
}

void OrbitalScene::enter()
{
   impl->active = true;
}

void OrbitalScene::exit()
{
   impl->active = false;
}

void OrbitalScene::reset()
{
   impl->resetWorld();
}

void OrbitalScene::applyInput(const InputState & input)
{
   impl->input = input;
}

void OrbitalScene::update(double dtSeconds)
{
   if (!impl->active)
      return;

   impl->secondsSinceEnter += dtSeconds;
   impl->dreamChaser->setThrusting(false);

   if (impl->input.rotateLeft)
      impl->dreamChaser->rotateLeft();
   if (impl->input.rotateRight)
      impl->dreamChaser->rotateRight();
   if (impl->input.mainThrust || impl->input.downPressed)
   {
      impl->dreamChaser->setThrusting(true);
      impl->dreamChaser->thrust(orbitalGetTime());
   }
   if (impl->input.firePressed)
      impl->projectiles.push_back(impl->dreamChaser->launchProjectile());

   const double t = orbitalGetTime();
   for (auto * object : impl->objects)
      object->update(t);
   for (auto & projectile : impl->projectiles)
      projectile.update(t);
   for (auto & part : impl->parts)
      part.update(t);
   for (auto & fragment : impl->fragments)
      fragment.update(t);

   impl->angleEarth -= ORBITAL_EARTH_SPEED;

   std::vector<OrbitalLegacyObject *> activeObjects;
   for (auto * object : impl->objects)
      if (object->isAlive())
         activeObjects.push_back(object);
   for (auto & part : impl->parts)
      if (part.isAlive())
         activeObjects.push_back(&part);

   for (auto & projectile : impl->projectiles)
   {
      if (!projectile.isAlive())
         continue;
      for (auto * object : activeObjects)
      {
         if (!object->isAlive())
            continue;
         if (orbitalDetectCollision(projectile.getPosition(),
                                    object->getPosition(),
                                    projectile.getRadius(),
                                    object->getRadius()))
         {
            projectile.onCollision(impl->parts, impl->fragments);
            object->onCollision(impl->parts, impl->fragments);
         }
      }
   }

   for (auto & fragment : impl->fragments)
   {
      if (!fragment.isAlive())
         continue;
      for (auto * object : activeObjects)
      {
         if (!object->isAlive())
            continue;
         if (orbitalDetectCollision(fragment.getPosition(),
                                    object->getPosition(),
                                    fragment.getRadius(),
                                    object->getRadius()))
         {
            fragment.onCollision(impl->parts, impl->fragments);
            object->onCollision(impl->parts, impl->fragments);
         }
      }
   }

   for (std::size_t i = 0; i < impl->parts.size(); ++i)
   {
      auto & partA = impl->parts[i];
      if (!partA.isAlive())
         continue;
      for (std::size_t j = i + 1; j < impl->parts.size(); ++j)
      {
         auto & partB = impl->parts[j];
         if (!partB.isAlive())
            continue;
         if (orbitalDetectCollision(partA.getPosition(), partB.getPosition(),
                                    partA.getRadius(), partB.getRadius()))
         {
            partA.onCollision(impl->parts, impl->fragments);
            partB.onCollision(impl->parts, impl->fragments);
         }
      }
   }

   for (std::size_t i = 0; i < impl->objects.size(); ++i)
   {
      auto * objectA = impl->objects[i];
      if (!objectA->isAlive())
         continue;
      for (std::size_t j = i + 1; j < impl->objects.size(); ++j)
      {
         auto * objectB = impl->objects[j];
         if (!objectB->isAlive())
            continue;
         if (orbitalDetectCollision(objectA->getPosition(), objectB->getPosition(),
                                    objectA->getRadius(), objectB->getRadius()))
         {
            objectA->onCollision(impl->parts, impl->fragments);
            objectB->onCollision(impl->parts, impl->fragments);
         }
      }
   }

   impl->projectiles.erase(std::remove_if(impl->projectiles.begin(), impl->projectiles.end(),
      [](const OrbitalLegacyProjectile & projectile)
      {
         return !projectile.isAlive();
      }),
      impl->projectiles.end());

   impl->parts.erase(std::remove_if(impl->parts.begin(), impl->parts.end(),
      [](const OrbitalLegacyPart & part)
      {
         return !part.isAlive();
      }),
      impl->parts.end());

   impl->fragments.erase(std::remove_if(impl->fragments.begin(), impl->fragments.end(),
      [](const OrbitalLegacyFragment & fragment)
      {
         return !fragment.isAlive();
      }),
      impl->fragments.end());

   std::ostringstream status;
   status << "Dream Chaser "
          << (impl->dreamChaser->isAlive() ? "active" : "destroyed")
          << ". Live bodies: " << impl->countLiveBodies()
          << ". Debris: " << impl->parts.size() + impl->fragments.size();
   impl->status = status.str();
}

double OrbitalScene::elapsedSeconds() const
{
   return impl->secondsSinceEnter;
}

std::size_t OrbitalScene::liveObjectCount() const
{
   return impl->countLiveBodies();
}

std::size_t OrbitalScene::fragmentCount() const
{
   return impl->fragments.size();
}

std::size_t OrbitalScene::projectileCount() const
{
   return impl->projectiles.size();
}

bool OrbitalScene::dreamChaserAlive() const
{
   return impl->dreamChaser->isAlive();
}

void OrbitalScene::render(ogstream & gout, const Position & viewport) const
{
   const Position mapBottomLeft(kMapMargin, kMapMargin);
   const Position mapTopRight(viewport.getX() - kOrbitPanelWidth - 28.0, viewport.getY() - 28.0);
   SceneRenderUtils::drawPanel(gout, mapBottomLeft, mapTopRight, 0.05, 0.07, 0.11);
   SceneRenderUtils::drawOutline(gout, mapBottomLeft, mapTopRight, 0.18, 0.24, 0.34);

   for (auto & star : const_cast<std::vector<BackgroundStar> &>(impl->backgroundStars))
      gout.drawStar(star.position, star.phase++);

   const Position earthCenter(viewport.getX() * 0.5, viewport.getY() * 0.5);
   OrbitalOgstream legacyGout;
   legacyGout.drawEarth(worldToLegacyScreen(OrbitalPosition(0.0, 0.0), viewport), impl->angleEarth);
   gout.setPosition(Position(earthCenter.getX() - 16.0, earthCenter.getY() - 38.0));
   gout << "Earth";

   const std::array<BodyVisual, 11> bodies =
   {{
      {&impl->gps1, "GPS-1"},
      {&impl->gps2, "GPS-2"},
      {&impl->gps3, "GPS-3"},
      {&impl->gps4, "GPS-4"},
      {&impl->gps5, "GPS-5"},
      {&impl->gps6, "GPS-6"},
      {&impl->sputnik, "Sputnik"},
      {&impl->hubble, "Hubble"},
      {&impl->dragon, "Dragon"},
      {&impl->starlink, "Starlink"},
      {impl->dreamChaser.get(), "DreamChaser"}
   }};

   for (std::size_t i = 0; i < bodies.size(); ++i)
   {
      const auto & body = bodies[i];
      if (!body.object->isAlive())
         continue;

      const Position screen = worldToScreen(body.object->getPosition(), viewport);
      const OrbitalPosition legacyScreen = worldToLegacyScreen(body.object->getPosition(), viewport);
      const double rotationRadians = body.object->getAngleShip() * (3.14159265358979323846 / 180.0);

      switch (i)
      {
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
         legacyGout.drawGPS(legacyScreen, rotationRadians);
         break;
      case 6:
         legacyGout.drawSputnik(legacyScreen, rotationRadians);
         break;
      case 7:
         legacyGout.drawHubble(legacyScreen, rotationRadians);
         break;
      case 8:
         legacyGout.drawCrewDragon(legacyScreen, rotationRadians);
         break;
      case 9:
         legacyGout.drawStarlink(legacyScreen, rotationRadians);
         break;
      case 10:
         legacyGout.drawShip(legacyScreen, rotationRadians, impl->dreamChaser->getThrusting());
         break;
      default:
         break;
      }

      if (i == 10)
         SceneRenderUtils::drawCrosshair(gout, screen, 7.0);
      if (i >= 6)
      {
         gout.setPosition(Position(screen.getX() + 8.0, screen.getY() + 8.0));
         gout << body.label;
      }
   }

   for (const auto & projectile : impl->projectiles)
      if (projectile.isAlive())
         legacyGout.drawProjectile(worldToLegacyScreen(projectile.getPosition(), viewport));

   for (const auto & part : impl->parts)
      if (part.isAlive())
      {
         const OrbitalPosition screen = worldToLegacyScreen(part.getPosition(), viewport);
         const double rotationRadians = part.getAngleShip() * (3.14159265358979323846 / 180.0);
         switch (part.getType())
         {
         case OrbitalLegacyPartType::HUBBLE_COMPUTER:
            legacyGout.drawHubbleComputer(screen, rotationRadians);
            break;
         case OrbitalLegacyPartType::HUBBLE_TELESCOPE:
            legacyGout.drawHubbleTelescope(screen, rotationRadians);
            break;
         case OrbitalLegacyPartType::HUBBLE_LEFT:
            legacyGout.drawHubbleLeft(screen, rotationRadians);
            break;
         case OrbitalLegacyPartType::HUBBLE_RIGHT:
            legacyGout.drawHubbleRight(screen, rotationRadians);
            break;
         case OrbitalLegacyPartType::GPS_CENTER:
            legacyGout.drawGPSCenter(screen, rotationRadians);
            break;
         case OrbitalLegacyPartType::GPS_LEFT:
            legacyGout.drawGPSLeft(screen, rotationRadians);
            break;
         case OrbitalLegacyPartType::GPS_RIGHT:
            legacyGout.drawGPSRight(screen, rotationRadians);
            break;
         case OrbitalLegacyPartType::STARLINK_BODY:
            legacyGout.drawStarlinkBody(screen, rotationRadians);
            break;
         case OrbitalLegacyPartType::STARLINK_ARRAY:
            legacyGout.drawStarlinkArray(screen, rotationRadians);
            break;
         case OrbitalLegacyPartType::DRAGON_CENTER:
            legacyGout.drawCrewDragonCenter(screen, rotationRadians);
            break;
         case OrbitalLegacyPartType::DRAGON_LEFT:
            legacyGout.drawCrewDragonLeft(screen, rotationRadians);
            break;
         case OrbitalLegacyPartType::DRAGON_RIGHT:
            legacyGout.drawCrewDragonRight(screen, rotationRadians);
            break;
         case OrbitalLegacyPartType::SPUTNIK_BODY:
         case OrbitalLegacyPartType::GENERIC:
         default:
            legacyGout.drawFragment(screen, rotationRadians);
            break;
         }
      }

   for (const auto & fragment : impl->fragments)
      if (fragment.isAlive())
         legacyGout.drawFragment(worldToLegacyScreen(fragment.getPosition(), viewport),
                                 fragment.getAngleShip() * (3.14159265358979323846 / 180.0));

   const Position rightColumnLeft(viewport.getX() - kOrbitPanelWidth, 24.0);
   const Position rightColumnRight(viewport.getX() - 24.0, viewport.getY() - 24.0);
   const Position topPanelBottom(rightColumnLeft.getX(), viewport.getY() - 184.0);
   const Position topPanelTop(rightColumnRight.getX(), viewport.getY() - 24.0);
   const Position middlePanelBottom(rightColumnLeft.getX(), viewport.getY() - 408.0);
   const Position middlePanelTop(rightColumnRight.getX(), viewport.getY() - 208.0);
   const Position bottomPanelBottom(rightColumnLeft.getX(), 24.0);
   const Position bottomPanelTop(rightColumnRight.getX(), 188.0);

   std::ostringstream altitude;
   altitude << std::fixed << std::setprecision(1)
            << "Dream Chaser alt: "
            << orbitalGetAltitude(impl->dreamChaser->getPosition()) / 1000.0
            << " km";
   std::ostringstream speed;
   speed << std::fixed << std::setprecision(1)
         << "Dream Chaser speed: "
         << impl->dreamChaser->getVelocity().getSpeed() / 1000.0
         << " km/s";
   SceneRenderUtils::drawInfoPanel(gout,
                                   topPanelBottom,
                                   topPanelTop,
                                   "Orbital Simulator",
                                   {
                                      "Live bodies: " + std::to_string(liveObjectCount()),
                                      "Projectiles: " + std::to_string(projectileCount()),
                                      "Parts: " + std::to_string(impl->parts.size()) +
                                         "  Fragments: " + std::to_string(fragmentCount()),
                                      altitude.str(),
                                      speed.str(),
                                      "Status: " + impl->status
                                   },
                                   0.10, 0.12, 0.16);

   std::string gpsState;
   gpsState += (impl->gps1.isAlive() ? "1" : "x");
   gpsState += (impl->gps2.isAlive() ? "1" : "x");
   gpsState += (impl->gps3.isAlive() ? "1" : "x");
   gpsState += (impl->gps4.isAlive() ? "1" : "x");
   gpsState += (impl->gps5.isAlive() ? "1" : "x");
   gpsState += (impl->gps6.isAlive() ? "1" : "x");
   SceneRenderUtils::drawInfoPanel(gout,
                                   middlePanelBottom,
                                   middlePanelTop,
                                   "Tracked Bodies",
                                   {
                                      std::string("Dream Chaser: ") +
                                         (impl->dreamChaser->isAlive() ? "alive" : "destroyed"),
                                      std::string("Hubble: ") +
                                         (impl->hubble.isAlive() ? "alive" : "destroyed"),
                                      std::string("Dragon: ") +
                                         (impl->dragon.isAlive() ? "alive" : "destroyed"),
                                      std::string("Starlink: ") +
                                         (impl->starlink.isAlive() ? "alive" : "destroyed"),
                                      std::string("Sputnik: ") +
                                         (impl->sputnik.isAlive() ? "alive" : "destroyed"),
                                      "GPS constellation: " + gpsState
                                   },
                                   0.12, 0.14, 0.20);

   SceneRenderUtils::drawLegendPanel(gout,
                                     bottomPanelBottom,
                                     bottomPanelTop,
                                     "Map Legend",
                                     {
                                        {"GPS satellites", 0.42, 0.78, 1.0},
                                        {"Dream Chaser", 1.00, 0.86, 0.22},
                                        {"Projectile", 1.0, 0.95, 0.28},
                                        {"Parts", 0.95, 0.55, 0.20},
                                        {"Fragments", 0.78, 0.78, 0.82}
                                     },
                                     0.10, 0.12, 0.17);
}
