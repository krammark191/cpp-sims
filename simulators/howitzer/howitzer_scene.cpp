#include "simulators/howitzer/howitzer_scene.h"

#include <algorithm>
#include <array>
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
#include "simulators/howitzer/howitzer_legacy_bridge.h"
#undef private

namespace
{
constexpr std::size_t kProjectileTrailSize = 20;
constexpr double kSimulationRate = 16.0;
constexpr double kCreamRed = 0.96;
constexpr double kCreamGreen = 0.93;
constexpr double kCreamBlue = 0.84;
constexpr double kGroundExplosionSeconds = 9.00;
constexpr double kTargetExplosionSeconds = 22.00;
constexpr double kTargetCompletionDelaySeconds = 2.0;
constexpr double kFireballSeconds = 6.0;

enum class HowitzerImpactType
{
   None,
   Ground,
   Target
};

double clamp01(double value)
{
   return value < 0.0 ? 0.0 : (value > 1.0 ? 1.0 : value);
}

HowitzerPosition makeViewportPosition(double width, double height)
{
   HowitzerPosition viewport;
   viewport.setPixelsX(width);
   viewport.setPixelsY(height);
   return viewport;
}

Position makeUiPosition(const HowitzerPosition & position)
{
   return Position(position.getPixelsX(), position.getPixelsY());
}

std::filesystem::path resolveHowitzerEffectDirectory(const std::string & name)
{
   return RuntimePaths::resolveRelativePath(std::filesystem::path("assets/howitzer/effects") / name);
}

const std::vector<ImageSequenceFrame> & groundExplosionFrames()
{
   static const std::vector<ImageSequenceFrame> frames =
      ImageSequenceLoader::loadPngSequence(resolveHowitzerEffectDirectory("explosion1").string());
   return frames;
}

const std::vector<ImageSequenceFrame> & targetExplosionFrames()
{
   static const std::vector<ImageSequenceFrame> frames =
      ImageSequenceLoader::loadPngSequence(resolveHowitzerEffectDirectory("explosion6").string());
   return frames;
}

const std::vector<ImageSequenceFrame> & fireballFrames()
{
   static const std::vector<ImageSequenceFrame> frames =
      ImageSequenceLoader::loadPngSpriteStrip(
         (resolveHowitzerEffectDirectory("fireball") / "fireball_strip.png").string(),
         64,
         64);
   return frames;
}

void drawImageFrame(ogstream & gout,
                    const ImageSequenceFrame & frame,
                    const Position & center,
                    double scale,
                    double rotationRadians = 0.0)
{
   if (frame.empty() || scale <= 0.0)
      return;

   const double width = static_cast<double>(frame.width) * scale;
   const double height = static_cast<double>(frame.height) * scale;
   const double cosAngle = std::cos(rotationRadians);
   const double sinAngle = std::sin(rotationRadians);
   const double halfWidth = width * 0.5;
   const double halfHeight = height * 0.5;

   for (int row = 0; row < frame.height; ++row)
   {
      for (int col = 0; col < frame.width; ++col)
      {
         const auto & pixel = frame.pixels[static_cast<std::size_t>(row * frame.width + col)];
         if (pixel.alpha <= 0.03)
            continue;

         const double localLeft = -halfWidth + static_cast<double>(col) * scale;
         const double localRight = localLeft + scale + 0.4;
         const double localTop = halfHeight - static_cast<double>(row) * scale;
         const double localBottom = localTop - scale;

         const auto rotatePoint =
            [&](double x, double y)
            {
               return Position(center.getX() + x * cosAngle - y * sinAngle,
                               center.getY() + x * sinAngle + y * cosAngle);
            };

         const Position p1 = rotatePoint(localLeft, localBottom);
         const Position p2 = rotatePoint(localLeft, localTop + 0.4);
         const Position p3 = rotatePoint(localRight, localTop + 0.4);
         const Position p4 = rotatePoint(localRight, localBottom);
         const double red = pixel.red * pixel.alpha;
         const double green = pixel.green * pixel.alpha;
         const double blue = pixel.blue * pixel.alpha;
         for (double t = 0.0; t <= 1.0; t += 0.34)
         {
            const Position leftEdge(p1.getX() + (p2.getX() - p1.getX()) * t,
                                    p1.getY() + (p2.getY() - p1.getY()) * t);
            const Position rightEdge(p4.getX() + (p3.getX() - p4.getX()) * t,
                                     p4.getY() + (p3.getY() - p4.getY()) * t);
            gout.drawLine(leftEdge, rightEdge, red, green, blue);
         }
      }
   }
}

void drawExplosionSequence(ogstream & gout,
                           const std::vector<ImageSequenceFrame> & frames,
                           const Position & center,
                           double elapsedSeconds,
                           double durationSeconds,
                           double scale,
                           double rotationRadians = 0.0)
{
   if (frames.empty() || durationSeconds <= 0.0 || elapsedSeconds < 0.0 || elapsedSeconds > durationSeconds)
      return;

   const double progress = clamp01(elapsedSeconds / durationSeconds);
   const std::size_t frameIndex = std::min(
      frames.size() - 1,
      static_cast<std::size_t>(progress * static_cast<double>(frames.size() - 1)));
   drawImageFrame(gout, frames[frameIndex], center, scale, rotationRadians);
}

Position howitzerMuzzlePosition(const HowitzerLegacyHowitzer & howitzer)
{
   const HowitzerPosition & base = howitzer.position;
   const double angle = howitzer.getElevation().getRadians();
   return Position(base.getPixelsX() + std::sin(angle) * 28.0,
                   base.getPixelsY() + std::cos(angle) * 28.0);
}

void drawGround(HowitzerGround & ground,
                HowitzerOgstream & gout,
                bool drawTargetMarker)
{
   for (HowitzerPosition pos(0.0, 1000.0);
        pos.getPixelsY() < ground.posUpperRight.getPixelsY();
        pos.addMetersY(1000.0))
   {
      HowitzerPosition posLeft(pos);
      HowitzerPosition posRight(pos);
      posRight.setPixelsX(ground.posUpperRight.getPixelsX());
      gout.drawLine(posLeft, posRight, 0.85, 0.85, 0.85);
   }

   const int width = static_cast<int>(ground.posUpperRight.getPixelsX());
   for (int i = 0; i < width; ++i)
   {
      HowitzerPosition posBottom;
      HowitzerPosition posTop;
      posBottom.setPixelsX(static_cast<double>(i));
      posTop.setPixelsX(static_cast<double>(i) + 1.0);
      posTop.setPixelsY(ground.ground[i]);
      gout.drawRectangle(posBottom, posTop, 0.6, 0.4, 0.2);
   }

   if (drawTargetMarker)
      gout.drawTarget(ground.getTarget());

   for (HowitzerPosition pos(1000.0, 0.0);
        pos.getPixelsX() < ground.posUpperRight.getPixelsX();
        pos.addMetersX(1000.0))
   {
      HowitzerPosition posBottom(pos);
      HowitzerPosition posTop(pos);
      posTop.addPixelsY(10);
      gout.drawLine(posTop, posBottom, 0.6, 0.6, 0.6);
   }

   for (HowitzerPosition pos(5000.0, 0.0);
        pos.getPixelsX() < ground.posUpperRight.getPixelsX();
        pos.addMetersX(5000.0))
   {
      HowitzerPosition posText(pos);
      posText.addPixelsY(15);
      posText.addPixelsX(-10);
      gout = posText;
      gout << static_cast<int>(pos.getMetersX() / 1000.0) << "km";
   }

   for (HowitzerPosition pos(0.0, 2000.0);
        pos.getPixelsY() < ground.posUpperRight.getPixelsY();
        pos.addMetersY(2000.0))
   {
      HowitzerPosition posText(pos);
      posText.addPixelsX(5);
      posText.addPixelsY(-2);
      gout = posText;
      gout << static_cast<int>(pos.getMetersY()) << "m";
   }
}

void carveGroundCrater(HowitzerGround & ground,
                       const HowitzerPosition & impactPosition,
                       double radiusPixels,
                       double depthPixels)
{
   const int width = static_cast<int>(ground.posUpperRight.getPixelsX());
   const int center = static_cast<int>(impactPosition.getPixelsX());
   const int left = std::max(1, center - static_cast<int>(radiusPixels * 1.25));
   const int right = std::min(width - 2, center + static_cast<int>(radiusPixels * 1.25));
   if (left >= right)
      return;

   std::vector<double> original(right - left + 1, 0.0);
   for (int x = left; x <= right; ++x)
      original[x - left] = ground.ground[x];

   for (int x = left; x <= right; ++x)
   {
      const double distance = std::abs(static_cast<double>(x) - impactPosition.getPixelsX());
      const double normalized = distance / radiusPixels;
      double carved = original[x - left];

      if (normalized <= 1.0)
      {
         const double carveProfile = std::sqrt(std::max(0.0, 1.0 - normalized * normalized));
         carved -= depthPixels * carveProfile;
      }

      ground.ground[x] = std::max(0.0, carved);
   }
}
}

struct HowitzerScene::Impl
{
   bool active = false;
   double secondsSinceEnter = 0.0;
   double flightTimeSeconds = 0.0;
   bool isFiring = false;
   bool isHit = false;
   bool hasShot = false;
   bool targetDestroyed = false;
   bool gameComplete = false;
   double fireEffectSeconds = kFireballSeconds;
   InputState input;
   HowitzerImpactType impactType = HowitzerImpactType::None;
   double impactEffectSeconds = 0.0;
   double completionDelaySeconds = 0.0;
   HowitzerPosition impactPosition;
   HowitzerPosition viewport = makeViewportPosition(1280.0, 720.0);
   HowitzerGround ground = HowitzerGround(viewport);
   HowitzerLegacyHowitzer howitzer;
   HowitzerLegacyProjectile projectile;
   std::array<HowitzerPosition, kProjectileTrailSize> projectileTrail {};
   std::string status = "Howitzer simulation ready.";

   Impl()
   {
      resetWorld();
   }

   void resetWorld()
   {
      secondsSinceEnter = 0.0;
      flightTimeSeconds = 0.0;
      isFiring = false;
      isHit = false;
      hasShot = false;
      targetDestroyed = false;
      gameComplete = false;
      fireEffectSeconds = kFireballSeconds;
      input = InputState{};
      impactType = HowitzerImpactType::None;
      impactEffectSeconds = 0.0;
      completionDelaySeconds = 0.0;
      impactPosition = HowitzerPosition();
      projectile.reset();
      for (auto & point : projectileTrail)
         point = HowitzerPosition();

      HowitzerPosition posHowitzer;
      posHowitzer.setPixelsX(viewport.getPixelsX() * 0.5);
      ground.reset(posHowitzer);
      howitzer = HowitzerLegacyHowitzer();
      howitzer.setPosition(posHowitzer);
      status = statusText();
   }

   std::string statusText() const
   {
      std::ostringstream out;
      out << std::fixed << std::setprecision(1)
          << "Angle " << howitzer.getElevation().getDegrees() << " deg. ";

      if (gameComplete)
      {
         out << "Target destroyed. Mission complete.";
      }
      else if (targetDestroyed)
      {
         out << "Target destroyed. Awaiting mission confirmation.";
      }
      else if (isFiring)
      {
         out << "Projectile in flight for " << flightTimeSeconds << " s.";
      }
      else if (hasShot)
      {
         out << (isHit ? "Target hit." : "Target missed.")
             << " Ready to fire again.";
      }
      else
      {
         out << "Ready to fire.";
      }

      return out.str();
   }

   void advanceTrail()
   {
      for (std::size_t i = projectileTrail.size() - 1; i > 0; --i)
         projectileTrail[i] = projectileTrail[i - 1];
      projectileTrail[0] = projectile.getPosition();
   }

   void startImpact(HowitzerImpactType type, const HowitzerPosition & position)
   {
      impactType = type;
      impactPosition = position;
      impactEffectSeconds = 0.0;

      if (type == HowitzerImpactType::Target)
         completionDelaySeconds = kTargetExplosionSeconds + kTargetCompletionDelaySeconds;
   }

   void updateImpact(double simulationSeconds)
   {
      if (impactType != HowitzerImpactType::None)
      {
         impactEffectSeconds += simulationSeconds;
         if (impactType == HowitzerImpactType::Ground &&
             impactEffectSeconds >= kGroundExplosionSeconds)
         {
            impactType = HowitzerImpactType::None;
         }
      }

      if (targetDestroyed && !gameComplete)
      {
         completionDelaySeconds -= simulationSeconds;
         if (completionDelaySeconds <= 0.0)
         {
            completionDelaySeconds = 0.0;
            gameComplete = true;
            impactType = HowitzerImpactType::None;
         }
      }
   }
};

HowitzerScene::HowitzerScene() :
   bindings(
   {
      {"Rotate left", "A", "Left Arrow"},
      {"Rotate right", "D", "Right Arrow"},
      {"Raise barrel", "W", "Up Arrow"},
      {"Lower barrel", "S", "Down Arrow"},
      {"Fire", "F", "Space"},
      {"Pause menu", "Esc", nullptr}
   }),
   impl(std::make_unique<Impl>())
{
}

HowitzerScene::~HowitzerScene() = default;

SimulatorId HowitzerScene::id() const
{
   return SimulatorId::Howitzer;
}

const char * HowitzerScene::sceneName() const
{
   return "HowitzerScene";
}

const char * HowitzerScene::statusText() const
{
   return impl->status.c_str();
}

const std::vector<ControlBinding> & HowitzerScene::keybindings() const
{
   return bindings;
}

bool HowitzerScene::supportsRecordedPreview() const
{
   return true;
}

bool HowitzerScene::supportsFutureAiModes() const
{
   return false;
}

bool HowitzerScene::isReady() const
{
   return true;
}

bool HowitzerScene::isPlaceholder() const
{
   return false;
}

void HowitzerScene::enter()
{
   impl->active = true;
}

void HowitzerScene::exit()
{
   impl->active = false;
}

void HowitzerScene::reset()
{
   impl->resetWorld();
}

void HowitzerScene::applyInput(const InputState & input)
{
   impl->input = input;
}

void HowitzerScene::update(double dtSeconds)
{
   if (!impl->active)
      return;

   const double simulationSeconds = dtSeconds * kSimulationRate;
   impl->secondsSinceEnter += simulationSeconds;

   if (!impl->targetDestroyed)
   {
      if (impl->input.rotateRight)
         impl->howitzer.rotate(0.05);
      if (impl->input.rotateLeft)
         impl->howitzer.rotate(-0.05);
      if (impl->input.upPressed)
         impl->howitzer.raise(0.003);
      if (impl->input.downPressed)
         impl->howitzer.raise(-0.003);
   }

   if (impl->input.firePressed && !impl->isFiring && !impl->targetDestroyed)
   {
      impl->flightTimeSeconds = 0.0;
      impl->projectile.reset();
      impl->projectile.fire(impl->howitzer.getPosition(),
                            impl->howitzer.getElevation(),
                            impl->howitzer.getMuzzleVelocity(),
                            impl->flightTimeSeconds);
      impl->isFiring = true;
      impl->isHit = false;
      impl->hasShot = true;
      impl->fireEffectSeconds = 0.0;
      for (auto & point : impl->projectileTrail)
         point = impl->projectile.getPosition();
   }

   if (impl->isFiring)
   {
      impl->flightTimeSeconds += simulationSeconds;
      impl->projectile.advance(impl->flightTimeSeconds);
      impl->advanceTrail();

      HowitzerPosition projectilePosition = impl->projectile.getPosition();
      const double groundElevation = impl->ground.getElevationMeters(projectilePosition);
      if (projectilePosition.getMetersY() < groundElevation)
      {
         projectilePosition.setMetersY(groundElevation);
         impl->isFiring = false;
         impl->isHit =
            std::abs(projectilePosition.getMetersX() -
                     impl->ground.getTarget().getMetersX()) < 175.0;

         if (impl->isHit)
         {
            impl->targetDestroyed = true;
            impl->startImpact(HowitzerImpactType::Target, impl->ground.getTarget());
         }
         else
         {
            carveGroundCrater(impl->ground, projectilePosition, 24.0, 18.0);
            impl->startImpact(HowitzerImpactType::Ground, projectilePosition);
         }

         impl->projectile.reset();
      }
   }

   impl->fireEffectSeconds += simulationSeconds;
   impl->updateImpact(simulationSeconds);
   impl->status = impl->statusText();
}

double HowitzerScene::elapsedSeconds() const
{
   return impl->secondsSinceEnter;
}

double HowitzerScene::elevationDegrees() const
{
   return impl->howitzer.getElevation().getDegrees();
}

bool HowitzerScene::projectileActive() const
{
   return impl->isFiring;
}

bool HowitzerScene::lastShotHitTarget() const
{
   return impl->isHit;
}

bool HowitzerScene::shotComplete() const
{
   return impl->gameComplete;
}

double HowitzerScene::targetDistanceMeters() const
{
   return std::abs(impl->ground.getTarget().getMetersX() - impl->howitzer.getPosition().getMetersX());
}

double HowitzerScene::projectileAltitudeMeters() const
{
   return impl->projectile.getPosition().getMetersY();
}

void HowitzerScene::render(ogstream & gout, const Position & viewport) const
{
   gout.drawRectangle(Position(0.0, 0.0),
                      viewport,
                      kCreamRed,
                      kCreamGreen,
                      kCreamBlue);

   HowitzerOgstream legacyGout;
   drawGround(impl->ground, legacyGout, !impl->targetDestroyed);
   impl->howitzer.draw(legacyGout, -1.0);

   if (impl->isFiring)
   {
      for (std::size_t i = 0; i < impl->projectileTrail.size(); ++i)
         legacyGout.drawProjectile(impl->projectileTrail[i], static_cast<double>(i) * 0.5);
   }

   drawExplosionSequence(gout,
                         fireballFrames(),
                         howitzerMuzzlePosition(impl->howitzer),
                         impl->fireEffectSeconds,
                         kFireballSeconds,
                         0.95,
                         -impl->howitzer.getElevation().getRadians() - 1.57079632679);

   if (impl->impactType == HowitzerImpactType::Ground)
   {
      drawExplosionSequence(gout,
                            groundExplosionFrames(),
                            Position(impl->impactPosition.getPixelsX(),
                                     impl->impactPosition.getPixelsY() + 10.0),
                            impl->impactEffectSeconds,
                            kGroundExplosionSeconds,
                            1.26);
   }
   else if (impl->impactType == HowitzerImpactType::Target)
   {
      drawExplosionSequence(gout,
                            targetExplosionFrames(),
                            Position(impl->impactPosition.getPixelsX(),
                                     impl->impactPosition.getPixelsY() + 18.0),
                            impl->impactEffectSeconds,
                            kTargetExplosionSeconds,
                            1.4);
   }

   std::ostringstream flightTime;
   flightTime << std::fixed << std::setprecision(1)
              << "Flight time: " << impl->flightTimeSeconds << " s";
   std::ostringstream angle;
   angle << std::fixed << std::setprecision(1)
         << "Howitzer angle: " << impl->howitzer.getElevation().getDegrees() << " deg";
   std::ostringstream target;
   target << std::fixed << std::setprecision(1)
          << "Target offset: " << targetDistanceMeters() / 1000.0 << " km";

   SceneRenderUtils::drawInfoPanel(gout,
                                   Position(78.0, viewport.getY() - 152.0),
                                   Position(408.0, viewport.getY() - 18.0),
                                   "Howitzer",
                                   {
                                      flightTime.str(),
                                      angle.str(),
                                      target.str(),
                                      std::string("Result: ") +
                                         (impl->targetDestroyed
                                            ? "Target destroyed"
                                            : (impl->hasShot
                                                  ? (impl->isHit ? "Hit" : (impl->isFiring ? "In flight" : "Miss"))
                                                  : "Ready")),
                                      impl->status
                                   },
                                   0.10, 0.12, 0.16);
}
