#include "simulators/lander/lander_scene.h"

#include <cmath>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "app/runtime_paths.h"
#include "engine/image_sequence.h"
#include "simulators/common/scene_render_utils.h"

#define private public
#include "simulators/lander/legacy/acceleration.h"
#include "simulators/lander/legacy/ground.h"
#include "simulators/lander/legacy/lander.h"
#include "simulators/lander/legacy/star.h"
#include "simulators/lander/legacy/thrust.h"
#undef private

namespace
{
constexpr double kGravityMetersPerSecondSquared = -1.625;
constexpr double kTimeStepSeconds = 0.1;
constexpr double kInitialFuelPounds = 5000.0;
constexpr double kHudTextScale = 0.13;

enum class CrashTier
{
   None,
   HardCrash,
   Breakup,
   MediumExplosion,
   LargeExplosion
};

struct SmokeParticle
{
   Position position;
   double velocityX = 0.0;
   double velocityY = 0.0;
   double size = 0.0;
   double widthScale = 1.0;
   double growthRate = 0.0;
   double driftPhase = 0.0;
   double heat = 0.0;
   double life = 0.0;
   double maxLife = 0.0;
   double shade = 0.0;
};

struct DebrisFragment
{
   Position position;
   double velocityX = 0.0;
   double velocityY = 0.0;
   double size = 0.0;
   double length = 0.0;
   double crossLength = 0.0;
   double glow = 0.0;
   double angle = 0.0;
   double spin = 0.0;
   double life = 0.0;
   double maxLife = 0.0;
};

double clamp01(double value)
{
   return value < 0.0 ? 0.0 : (value > 1.0 ? 1.0 : value);
}

Position offsetPosition(const Position & origin, double angle, double distance)
{
   return Position(origin.getX() + std::cos(angle) * distance,
                   origin.getY() + std::sin(angle) * distance);
}

std::filesystem::path apolloExplosionAssetDirectory()
{
   return RuntimePaths::resolveRelativePath("assets/apollo/effects/explosion4");
}

const std::vector<ImageSequenceFrame> & apolloExplosionFrames()
{
   static const std::vector<ImageSequenceFrame> frames =
      ImageSequenceLoader::loadPngSequence(apolloExplosionAssetDirectory().string());
   return frames;
}

bool hasApolloExplosionFrames()
{
   return !apolloExplosionFrames().empty();
}

double approximateHudTextWidth(const std::string & text, double scale)
{
   return static_cast<double>(text.size()) * 104.0 * scale;
}

Position centeredHudAnchor(double centerX, double y, const std::string & text, double scale)
{
   return Position(centerX - approximateHudTextWidth(text, scale) * 0.5, y);
}

void drawImageFrame(ogstream & gout,
                    const ImageSequenceFrame & frame,
                    const Position & center,
                    double scale)
{
   if (frame.empty() || scale <= 0.0)
      return;

   const double width = static_cast<double>(frame.width) * scale;
   const double height = static_cast<double>(frame.height) * scale;
   const Position bottomLeft(center.getX() - width * 0.5,
                             center.getY() - height * 0.5);

   for (int row = 0; row < frame.height; ++row)
   {
      for (int col = 0; col < frame.width; ++col)
      {
         const auto & pixel = frame.pixels[static_cast<std::size_t>(row * frame.width + col)];
         if (pixel.alpha <= 0.03)
            continue;

         const double left = bottomLeft.getX() + static_cast<double>(col) * scale;
         const double top = bottomLeft.getY() + height - static_cast<double>(row) * scale;
         const double red = pixel.red * pixel.alpha;
         const double green = pixel.green * pixel.alpha;
         const double blue = pixel.blue * pixel.alpha;
         gout.drawRectangle(Position(left, top - scale),
                            Position(left + scale + 0.4, top + 0.4),
                            red,
                            green,
                            blue);
      }
   }
}

void drawSmokePuff(ogstream & gout, const SmokeParticle & smoke)
{
   const double fade =
      smoke.maxLife > 0.0 ? clamp01(smoke.life / smoke.maxLife) : 0.0;
   if (fade <= 0.0)
      return;

   const double dust = smoke.shade * (0.25 + 0.45 * fade);
   const double heatGlow = smoke.heat * fade;

   gout.drawRectangle(Position(smoke.position.getX() - smoke.size * 0.55,
                               smoke.position.getY() - smoke.size * 0.45),
                      Position(smoke.position.getX() + smoke.size * 0.55,
                               smoke.position.getY() + smoke.size * 0.45),
                      clamp01(dust + heatGlow * 0.42),
                      clamp01(dust + heatGlow * 0.20),
                      clamp01(dust));
   if (heatGlow > 0.10)
   {
      gout.drawRectangle(Position(smoke.position.getX() - smoke.size * 0.22,
                                  smoke.position.getY() - smoke.size * 0.18),
                         Position(smoke.position.getX() + smoke.size * 0.22,
                                  smoke.position.getY() + smoke.size * 0.18),
                         clamp01(0.95 * fade),
                         clamp01(0.86 * fade),
                         clamp01(0.62 * fade));
   }
}

void drawDebrisShard(ogstream & gout, const DebrisFragment & fragment)
{
   const double fade =
      fragment.maxLife > 0.0 ? clamp01(fragment.life / fragment.maxLife) : 0.0;
   const double visualScale = 0.72;
   const double bodyRed = 0.82 * fade + fragment.glow * 0.12;
   const double bodyGreen = 0.78 * fade + fragment.glow * 0.06;
   const double bodyBlue = 0.72 * fade;
   const Position front = offsetPosition(fragment.position, fragment.angle, fragment.length * visualScale);
   const Position rear = offsetPosition(fragment.position,
                                        fragment.angle + 3.141592653589793,
                                        fragment.length * 0.65 * visualScale);
   const Position wingA = offsetPosition(fragment.position,
                                         fragment.angle + 1.5707963267948966,
                                         fragment.crossLength * visualScale);
   const Position wingB = offsetPosition(fragment.position,
                                         fragment.angle - 1.5707963267948966,
                                         fragment.crossLength * 0.85 * visualScale);
   const Position spark = offsetPosition(fragment.position,
                                         fragment.angle + 0.42,
                                         fragment.length * 0.45 * visualScale);

   gout.drawLine(rear, front, bodyRed, bodyGreen, bodyBlue);
   gout.drawLine(wingA, wingB,
                 clamp01(bodyRed + 0.10),
                 clamp01(bodyGreen + 0.08),
                 clamp01(bodyBlue + 0.04));
   gout.drawRectangle(Position(fragment.position.getX() - fragment.size * 0.24,
                               fragment.position.getY() - fragment.size * 0.21),
                      Position(fragment.position.getX() + fragment.size * 0.24,
                               fragment.position.getY() + fragment.size * 0.21),
                      clamp01(0.72 * fade + fragment.glow * 0.18),
                      clamp01(0.68 * fade + fragment.glow * 0.10),
                      clamp01(0.62 * fade));
   if (fragment.glow > 0.18 && fade > 0.30)
   {
      gout.drawLine(fragment.position,
                    spark,
                    0.98 * fade,
                    0.74 * fade,
                    0.26 * fade);
   }
}

void drawExplosionBurst(ogstream & gout,
                        const Position & impactPosition,
                        CrashTier crashTier,
                        double crashEffectSeconds,
                        double ejectaBias)
{
   const double duration =
      crashTier == CrashTier::LargeExplosion ? 0.34 : 0.26;
   if (crashEffectSeconds > duration)
      return;

   const double progress = clamp01(crashEffectSeconds / duration);
   const double inverse = 1.0 - progress;
   const double baseRadius =
      crashTier == CrashTier::LargeExplosion ? 48.0 : 30.0;
   const double flashRadius = baseRadius * (0.30 + progress * 0.30);
   const double ejectaReach = baseRadius * (0.65 + progress * 0.75);
   const double biasOffset = baseRadius * 0.18 * ejectaBias;

   SceneRenderUtils::drawMarker(gout,
                                Position(impactPosition.getX() + biasOffset * 0.30,
                                         impactPosition.getY()),
                                flashRadius,
                                1.0,
                                0.96 * inverse,
                                0.78 * inverse);
   SceneRenderUtils::drawMarker(gout,
                                Position(impactPosition.getX() + biasOffset * 0.55,
                                         impactPosition.getY() + baseRadius * 0.05),
                                flashRadius * 0.52,
                                1.0,
                                0.90 * inverse,
                                0.58 * inverse);

   for (int i = 0; i < 12; ++i)
   {
      const double angle = 0.12 + progress * 0.25 + i * 0.5235987755982988 +
         ejectaBias * 0.18;
      const double startDistance = flashRadius * (0.30 + 0.06 * (i % 2));
      const double directionalBoost =
         1.0 + ejectaBias * std::cos(angle) * 0.24;
      const double endDistance =
         ejectaReach * (0.86 + 0.14 * std::sin(i + progress * 6.0)) * directionalBoost;
      const Position start = offsetPosition(Position(impactPosition.getX() + biasOffset * 0.22,
                                                     impactPosition.getY()),
                                            angle,
                                            startDistance);
      const Position end = offsetPosition(Position(impactPosition.getX() + biasOffset * 0.46,
                                                   impactPosition.getY()),
                                          angle,
                                          endDistance);
      gout.drawLine(start,
                    end,
                    0.96 * inverse,
                    0.88 * inverse,
                    0.70 * inverse);
   }

   for (int i = 0; i < 5; ++i)
   {
      const double flareAngle = i * 1.2566370614359172 + progress * 0.8 + ejectaBias * 0.10;
      const double flareRadius = ejectaReach * (0.38 + 0.10 * i) *
         (1.0 + ejectaBias * std::cos(flareAngle) * 0.14);
      const Position flareCenter = offsetPosition(Position(impactPosition.getX() + biasOffset * 0.28,
                                                           impactPosition.getY()),
                                                  flareAngle,
                                                  flareRadius * 0.25);
      SceneRenderUtils::drawMarker(gout,
                                   flareCenter,
                                   flareRadius * 0.08 * inverse,
                                   0.92 * inverse,
                                   0.88 * inverse,
                                   0.78 * inverse);
   }
}

void drawExplosionAsset(ogstream & gout,
                        const Position & impactPosition,
                        CrashTier crashTier,
                        double crashEffectSeconds,
                        double ejectaBias)
{
   const auto & frames = apolloExplosionFrames();
   if (frames.empty())
      return;

   const double duration =
      crashTier == CrashTier::LargeExplosion ? 4.96 : 4.00;
   if (crashEffectSeconds > duration)
      return;

   const double progress = clamp01(crashEffectSeconds / duration);
   const std::size_t frameIndex = std::min(
      frames.size() - 1,
      static_cast<std::size_t>(progress * static_cast<double>(frames.size() - 1)));
   const Position spriteCenter(impactPosition.getX() + ejectaBias * 8.0,
                               impactPosition.getY() + (crashTier == CrashTier::LargeExplosion ? 34.0 : 24.0));
   const double scale = crashTier == CrashTier::LargeExplosion ? 2.70 : 2.05;
   drawImageFrame(gout, frames[frameIndex], spriteCenter, scale);
}

double fuelRatio(double fuelPounds)
{
   const double ratio = fuelPounds / kInitialFuelPounds;
   return ratio < 0.0 ? 0.0 : (ratio > 1.0 ? 1.0 : ratio);
}

void velocityColor(double velocity,
                   double & red,
                   double & green,
                   double & blue)
{
   if (velocity >= 10.0)
   {
      red = 0.96;
      green = 0.24;
      blue = 0.18;
      return;
   }

   if (velocity >= 3.0)
   {
      red = 0.96;
      green = 0.82;
      blue = 0.18;
      return;
   }

   red = 0.26;
   green = 0.90;
   blue = 0.38;
}

CrashTier classifyCrashTier(double speed)
{
   if (speed >= 20.0)
      return CrashTier::LargeExplosion;
   if (speed >= 10.0)
      return CrashTier::MediumExplosion;
   if (speed >= 3.0)
      return CrashTier::Breakup;
   return CrashTier::HardCrash;
}
}

struct LanderScene::Impl
{
   Position viewport;
   Ground ground;
   Lander lander;
   Thrust thrust;
   std::vector<Star> stars;
   Position impactPosition;
   double impactSpeed = 0.0;
   double impactVelocityX = 0.0;
   double impactVelocityY = 0.0;
   double crashEffectSeconds = 0.0;
   double craterRadius = 0.0;
   CrashTier crashTier = CrashTier::None;
   std::vector<SmokeParticle> smokeParticles;
   std::vector<DebrisFragment> debrisFragments;
   bool active;
   double secondsSinceEnter;

   Impl() :
      viewport(1280.0, 720.0),
      ground(viewport),
      lander(viewport),
      thrust(),
      stars(),
      active(false),
      secondsSinceEnter(0.0)
   {
      thrust.mainEngine = false;
      thrust.clockwise = false;
      thrust.counterClockwise = false;
      ground.reset();
      lander.reset(viewport);

      const int starCount = static_cast<int>(viewport.getX() / 8.0);
      stars.reserve(starCount);
      for (int i = 0; i < starCount; ++i)
      {
         Star star;
         star.reset(static_cast<int>(viewport.getX()), static_cast<int>(viewport.getY()));
         stars.push_back(star);
      }
   }

   void resetCrashEffects()
   {
      impactPosition = Position();
      impactSpeed = 0.0;
      impactVelocityX = 0.0;
      impactVelocityY = 0.0;
      crashEffectSeconds = 0.0;
      craterRadius = 0.0;
      crashTier = CrashTier::None;
      smokeParticles.clear();
      debrisFragments.clear();
   }

   void spawnSmoke(int count,
                   double sizeMin,
                   double sizeMax,
                   double lifeMin,
                   double lifeMax,
                   double riseMin,
                   double riseMax,
                   double spread)
   {
      for (int i = 0; i < count; ++i)
      {
         SmokeParticle particle;
         particle.position = impactPosition;
         particle.position.addX(random(-spread, spread));
         particle.position.addY(random(0.0, spread * 0.35));
         particle.velocityX = random(-spread * 0.55, spread * 0.55);
         particle.velocityY = random(riseMin, riseMax);
         particle.size = random(sizeMin, sizeMax);
         particle.widthScale = random(0.85, 1.60);
         particle.growthRate = random(1.4, 3.6);
         particle.driftPhase = random(0.0, 6.283185307179586);
         particle.heat = random(0.10, 0.42);
         particle.maxLife = random(lifeMin, lifeMax);
         particle.life = particle.maxLife;
         particle.shade = random(0.22, 0.58);
         smokeParticles.push_back(particle);
      }
   }

   void spawnDebris(int count,
                    double speedMin,
                    double speedMax,
                    double sizeMin,
                    double sizeMax)
   {
      for (int i = 0; i < count; ++i)
      {
         DebrisFragment fragment;
         fragment.position = impactPosition;
         fragment.position.addX(random(-8.0, 8.0));
         fragment.position.addY(random(2.0, 10.0));
         fragment.velocityX = random(-speedMax, speedMax);
         fragment.velocityY = random(speedMin, speedMax);
         fragment.size = random(sizeMin, sizeMax);
         fragment.length = fragment.size * random(1.15, 2.55);
         fragment.crossLength = fragment.size * random(0.35, 0.95);
         fragment.glow = random(0.08, 0.80);
         fragment.angle = random(0.0, 6.283185307179586);
         fragment.spin = random(-5.0, 5.0);
         fragment.maxLife = random(1.0, 2.4);
         fragment.life = fragment.maxLife;
         debrisFragments.push_back(fragment);
      }
   }

   void captureCrash(double speed)
   {
      impactSpeed = speed;
      impactVelocityX = lander.velocity.getDX();
      impactVelocityY = lander.velocity.getDY();
      impactPosition = lander.getPosition();
      impactPosition.setY(ground.getElevation(impactPosition));
      crashEffectSeconds = 0.0;
      crashTier = classifyCrashTier(speed);
      smokeParticles.clear();
      debrisFragments.clear();
      craterRadius = 0.0;

      switch (crashTier)
      {
      case CrashTier::LargeExplosion:
         craterRadius = lander.getWidth() * 1.25;
         spawnSmoke(42, 4.0, 12.0, 1.6, 3.1, 20.0, 56.0, 26.0);
         spawnDebris(8, 18.0, 42.0, 2.0, 5.5);
         break;
      case CrashTier::MediumExplosion:
         craterRadius = lander.getWidth() * 0.82;
         spawnSmoke(28, 3.0, 8.0, 1.2, 2.4, 16.0, 40.0, 18.0);
         spawnDebris(6, 12.0, 28.0, 1.5, 4.2);
         break;
      case CrashTier::Breakup:
         spawnSmoke(12, 2.0, 5.0, 0.9, 1.8, 8.0, 22.0, 12.0);
         spawnDebris(7, 10.0, 24.0, 2.0, 5.0);
         break;
      case CrashTier::HardCrash:
         spawnSmoke(8, 1.8, 4.2, 0.7, 1.5, 5.0, 16.0, 9.0);
         break;
      case CrashTier::None:
         break;
      }

      if (craterRadius > 0.0)
      {
         const int width = static_cast<int>(viewport.getX());
         const int center = static_cast<int>(impactPosition.getX());
         const double radius = craterRadius;
         const double depth = radius * (crashTier == CrashTier::LargeExplosion ? 0.82 : 0.60);
         const int left = std::max(1, center - static_cast<int>(radius * 1.45));
         const int right = std::min(width - 2, center + static_cast<int>(radius * 1.45));
         std::vector<double> original(right - left + 1, 0.0);

         for (int x = left; x <= right; ++x)
            original[x - left] = ground.ground[x];

         for (int x = left; x <= right; ++x)
         {
            const double distance = std::abs(x - impactPosition.getX());
            const double normalized = distance / radius;
            double carvedHeight = original[x - left];

            if (normalized <= 1.0)
            {
               // Carve a direct crater bowl out of the terrain rather than displacing it.
               const double carveProfile = std::sqrt(std::max(0.0, 1.0 - normalized * normalized));
               carvedHeight -= depth * carveProfile;
            }

            ground.ground[x] = std::max(4.0, carvedHeight);
         }

         std::vector<double> smoothed = original;
         for (int iteration = 0; iteration < 1; ++iteration)
         {
            for (int x = left + 1; x < right; ++x)
            {
               smoothed[x - left] =
                  (ground.ground[x - 1] + ground.ground[x] * 2.0 + ground.ground[x + 1]) * 0.25;
            }
            for (int x = left + 1; x < right; ++x)
               ground.ground[x] = smoothed[x - left];
         }
      }
   }

   void updateCrashEffects(double dtSeconds)
   {
      if (crashTier == CrashTier::None)
         return;

      crashEffectSeconds += dtSeconds;

      for (auto & particle : smokeParticles)
      {
         particle.position.addX(particle.velocityX * dtSeconds);
         particle.position.addY(particle.velocityY * dtSeconds);
         particle.velocityY += kGravityMetersPerSecondSquared * 3.4 * dtSeconds;
         particle.velocityX *= 0.998;
         particle.size += dtSeconds * particle.growthRate * 0.35;
         particle.widthScale += dtSeconds * 0.10;
         particle.driftPhase = std::atan2(particle.velocityY, particle.velocityX == 0.0 ? 0.0001 : particle.velocityX);
         particle.heat = std::max(0.0, particle.heat - dtSeconds * 0.42);
         particle.life -= dtSeconds;

         const double groundHeight = ground.getElevation(particle.position);
         if (particle.position.getY() <= groundHeight)
         {
            particle.position.setY(groundHeight);
            particle.velocityX *= 0.62;
            particle.velocityY = std::abs(particle.velocityY) * 0.22;
            particle.heat *= 0.55;
         }
      }

      for (auto & fragment : debrisFragments)
      {
         fragment.position.addX(fragment.velocityX * dtSeconds);
         fragment.position.addY(fragment.velocityY * dtSeconds);
         fragment.velocityY -= 18.0 * dtSeconds;
         fragment.velocityX *= 0.995;
         fragment.angle += fragment.spin * dtSeconds;
         fragment.life -= dtSeconds;

         const double groundHeight = ground.getElevation(fragment.position);
         if (fragment.position.getY() <= groundHeight)
         {
            fragment.position.setY(groundHeight);
            fragment.velocityX *= 0.4;
            fragment.velocityY = 0.0;
            fragment.spin *= 0.5;
         }
      }

      smokeParticles.erase(
         std::remove_if(smokeParticles.begin(),
                        smokeParticles.end(),
                        [](const SmokeParticle & particle)
                        {
                           return particle.life <= 0.0;
                        }),
         smokeParticles.end());

      debrisFragments.erase(
         std::remove_if(debrisFragments.begin(),
                        debrisFragments.end(),
                        [](const DebrisFragment & fragment)
                        {
                           return fragment.life <= 0.0;
                        }),
         debrisFragments.end());
   }
};

LanderScene::LanderScene() :
   bindings(
   {
      {"Rotate left", "Left Arrow", "A"},
      {"Rotate right", "Right Arrow", "D"},
      {"Main thruster", "Up Arrow", "W"},
      {"Pause menu", "Esc", nullptr}
   }),
   impl(std::make_unique<Impl>())
{
}

LanderScene::~LanderScene() = default;

SimulatorId LanderScene::id() const
{
   return SimulatorId::ApolloLander;
}

const char * LanderScene::sceneName() const
{
   return "LanderScene";
}

const char * LanderScene::statusText() const
{
   if (hasLanded())
      return "Apollo 11 legacy lander is active. Landing successful.";

   if (hasCrashed())
      return "Apollo 11 legacy lander is active. Crash detected.";

   return "Apollo 11 legacy lander is active. Real flight, fuel, and collision logic are wired.";
}

const std::vector<ControlBinding> & LanderScene::keybindings() const
{
   return bindings;
}

bool LanderScene::supportsRecordedPreview() const
{
   return true;
}

bool LanderScene::supportsFutureAiModes() const
{
   return false;
}

bool LanderScene::isReady() const
{
   return true;
}

bool LanderScene::isPlaceholder() const
{
   return false;
}

void LanderScene::enter()
{
   impl->active = true;
}

void LanderScene::exit()
{
   impl->active = false;
}

void LanderScene::reset()
{
   impl->secondsSinceEnter = 0.0;
   impl->thrust.mainEngine = false;
   impl->thrust.clockwise = false;
   impl->thrust.counterClockwise = false;
   impl->ground.reset();
   impl->lander.reset(impl->viewport);
   impl->resetCrashEffects();
}

void LanderScene::applyInput(const InputState & input)
{
   impl->thrust.mainEngine = input.mainThrust;
   impl->thrust.clockwise = input.rotateLeft;
   impl->thrust.counterClockwise = input.rotateRight;
}

void LanderScene::update(double dtSeconds)
{
   if (!impl->active)
      return;

   while (dtSeconds > 0.0)
   {
      const double step = dtSeconds > kTimeStepSeconds ? kTimeStepSeconds : dtSeconds;
      impl->secondsSinceEnter += step;

      if (impl->lander.isFlying())
      {
         Acceleration acceleration = impl->lander.input(impl->thrust, kGravityMetersPerSecondSquared);
         impl->lander.coast(acceleration, step);

         if (impl->ground.onPlatform(impl->lander.getPosition(), impl->lander.getWidth()) &&
             impl->lander.getSpeed() < 3.0)
         {
            impl->lander.land();
         }
         else if (impl->ground.hitGround(impl->lander.getPosition(), impl->lander.getWidth()))
         {
            impl->captureCrash(impl->lander.getSpeed());
            impl->lander.crash();
         }
      }

      impl->updateCrashEffects(step);

      dtSeconds -= step;
   }
}

double LanderScene::elapsedSeconds() const
{
   return impl->secondsSinceEnter;
}

bool LanderScene::isActive() const
{
   return impl->active;
}

double LanderScene::demoAltitudeMeters() const
{
   return impl->lander.getPosition().getY() -
      impl->ground.getElevation(impl->lander.getPosition());
}

double LanderScene::demoFuelPounds() const
{
   return static_cast<double>(impl->lander.getFuel());
}

double LanderScene::demoVelocityMetersPerSecond() const
{
   return impl->lander.getSpeed();
}

bool LanderScene::isFlying() const
{
   return impl->lander.isFlying();
}

bool LanderScene::hasLanded() const
{
   return impl->lander.isLanded();
}

bool LanderScene::hasCrashed() const
{
   return impl->lander.isDead();
}

bool LanderScene::hasCrashEffects() const
{
   return impl->crashTier != CrashTier::None;
}

void LanderScene::render(ogstream & gout) const
{
   for (auto & star : impl->stars)
      star.draw(gout);

   impl->ground.draw(gout);

   if (impl->craterRadius > 0.0)
   {
      const double deformedFloor = impl->ground.getElevation(impl->impactPosition);
      gout.drawRectangle(Position(impl->impactPosition.getX() - impl->craterRadius * 0.44,
                                  deformedFloor - 0.8),
                         Position(impl->impactPosition.getX() + impl->craterRadius * 0.44,
                                  deformedFloor + 1.4),
                         0.12, 0.08, 0.05);
   }

   const bool drawIntactLander =
      !hasCrashed() ||
      impl->crashTier == CrashTier::HardCrash ||
      impl->crashTier == CrashTier::None;
   if (drawIntactLander)
      impl->lander.draw(impl->thrust, gout);

   for (const auto & smoke : impl->smokeParticles)
      drawSmokePuff(gout, smoke);

   if ((impl->crashTier == CrashTier::LargeExplosion ||
        impl->crashTier == CrashTier::MediumExplosion) &&
       impl->crashEffectSeconds <= 0.92)
   {
      const double ejectaBias =
         clamp01(std::abs(impl->impactVelocityX) / 22.0) *
         (impl->impactVelocityX >= 0.0 ? 1.0 : -1.0);
      if (hasApolloExplosionFrames())
      {
         drawExplosionAsset(gout,
                            impl->impactPosition,
                            impl->crashTier,
                            impl->crashEffectSeconds,
                            ejectaBias);
      }
      else
      {
         drawExplosionBurst(gout,
                            impl->impactPosition,
                            impl->crashTier,
                            impl->crashEffectSeconds,
                            ejectaBias);
      }
   }

   for (const auto & fragment : impl->debrisFragments)
      drawDebrisShard(gout, fragment);

   const double altitude =
      impl->lander.getPosition().getY() - impl->ground.getElevation(impl->lander.getPosition());
   const double fuel = impl->lander.getFuel();
   const double velocityMetersPerSecond = impl->lander.getSpeed();
   const double pulse = 0.5 + 0.5 * std::sin(impl->secondsSinceEnter * 4.5);
   const double fuelFill = fuelRatio(fuel);

   std::ostringstream velocityText;
   velocityText << std::fixed << std::setprecision(2)
                << velocityMetersPerSecond << " m/s";
   double velocityRed = 1.0;
   double velocityGreen = 1.0;
   double velocityBlue = 1.0;
   velocityColor(velocityMetersPerSecond, velocityRed, velocityGreen, velocityBlue);

   const Position velocityAnchor(impl->viewport.getX() - 290.0, impl->viewport.getY() - 44.0);
   gout.drawHudText(velocityAnchor,
                    "VELOCITY " + velocityText.str(),
                    kHudTextScale,
                    velocityRed,
                    velocityGreen,
                    velocityBlue);

   const double barWidth = 320.0;
   const double barHeight = 22.0;
   const double barLeft = (impl->viewport.getX() - barWidth) * 0.5;
   const double barBottom = impl->viewport.getY() - 62.0;
   const double barCenterX = barLeft + barWidth * 0.5;
   const std::string fuelLabel = "FUEL";
   gout.drawHudText(centeredHudAnchor(barCenterX,
                                      impl->viewport.getY() - 28.0,
                                      fuelLabel,
                                      0.10),
                    fuelLabel,
                    0.10,
                    0.95,
                    0.95,
                    0.90);
   gout.drawRectangle(Position(barLeft, barBottom),
                      Position(barLeft + barWidth, barBottom + barHeight),
                      0.12, 0.13, 0.15);
   gout.drawRectangle(Position(barLeft + 3.0, barBottom + 3.0),
                      Position(barLeft + 3.0 + (barWidth - 6.0) * fuelFill, barBottom + barHeight - 3.0),
                      fuelFill > 0.25 ? 0.92 : 0.95,
                      fuelFill > 0.25 ? 0.78 : 0.22,
                      fuelFill > 0.25 ? 0.20 : 0.18);
   SceneRenderUtils::drawOutline(gout,
                                 Position(barLeft, barBottom),
                                 Position(barLeft + barWidth, barBottom + barHeight),
                                 0.90, 0.90, 0.86);

   if (fuel <= 0.0)
   {
      const std::string noFuelLabel = "NO FUEL";
      gout.drawHudText(centeredHudAnchor(barCenterX,
                                         impl->viewport.getY() - 102.0,
                                         noFuelLabel,
                                         kHudTextScale),
                       noFuelLabel,
                       kHudTextScale,
                       1.0,
                       0.25 + 0.55 * pulse,
                       0.25 + 0.55 * pulse);
   }
   else if (fuelFill <= 0.25)
   {
      const std::string lowFuelLabel = "FUEL LOW";
      gout.drawHudText(centeredHudAnchor(barCenterX,
                                         impl->viewport.getY() - 102.0,
                                         lowFuelLabel,
                                         kHudTextScale),
                       lowFuelLabel,
                       kHudTextScale,
                       1.0,
                       0.45 + 0.45 * pulse,
                       0.20 + 0.40 * pulse);
   }

   SceneRenderUtils::drawInfoPanel(gout,
                                   Position(18.0, impl->viewport.getY() - 112.0),
                                   Position(250.0, impl->viewport.getY() - 18.0),
                                   "Apollo 11",
                                   {
                                      "Altitude: " + std::to_string(static_cast<int>(altitude)) + " meters"
                                   },
                                   0.10, 0.12, 0.16);

   if (hasLanded())
   {
      SceneRenderUtils::drawCenteredBanner(gout,
                                           impl->viewport,
                                           impl->viewport.getY() * 0.70,
                                           "The Eagle has landed!");
   }
   else if (hasCrashed())
   {
      SceneRenderUtils::drawCenteredBanner(gout,
                                           impl->viewport,
                                           impl->viewport.getY() * 0.70,
                                           "Houston, we have a problem!");
   }
}
