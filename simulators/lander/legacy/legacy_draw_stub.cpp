#include "uiDraw.h"

#include <cmath>
#include <random>

namespace
{
std::mt19937 & generator()
{
   static std::mt19937 rng(23011);
   return rng;
}
}

void ogstream::flush()
{
   str("");
   clear();
}

void ogstream::drawLander(const Position &, double)
{
}

void ogstream::drawLanderFlames(const Position &, double, bool, bool, bool)
{
}

void ogstream::drawStar(const Position &, unsigned char)
{
}

void ogstream::drawRectangle(const Position &, const Position &, double, double, double) const
{
}

void ogstream::drawLine(const Position &, const Position &, double, double, double) const
{
}

void ogstream::drawHudText(const Position &, const std::string &, double, double, double, double) const
{
}

Position ogstream::rotate(const Position & posOrigin, double x, double y, double rotation) const
{
   const double cosRotation = std::cos(rotation);
   const double sinRotation = std::sin(rotation);
   return Position(posOrigin.getX() + x * cosRotation - y * sinRotation,
                   posOrigin.getY() + x * sinRotation + y * cosRotation);
}

void ogstream::drawText(const Position &, const char *) const
{
}

int random(int min, int max)
{
   std::uniform_int_distribution<int> distribution(min, max);
   return distribution(generator());
}

double random(double min, double max)
{
   std::uniform_real_distribution<double> distribution(min, max);
   return distribution(generator());
}
