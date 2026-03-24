#include "uiDraw.h"

#include <random>

namespace
{
std::mt19937 & generator()
{
   static std::mt19937 rng(23012);
   return rng;
}
}

void ogstream::flush()
{
   str("");
   clear();
}

void ogstream::drawLine(const Position &, const Position &, double, double, double)
{
}

void ogstream::drawRectangle(const Position &, const Position &, double, double, double)
{
}

void ogstream::drawProjectile(const Position &, double)
{
}

void ogstream::drawHowitzer(const Position &, double, double)
{
}

void ogstream::drawTarget(const Position &)
{
}

void ogstream::drawText(const Position &, const char *)
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
