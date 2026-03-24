/***********************************************************************
 * Header File:
 *    STAR
 * Author:
 *    Br. Helfrich
 * Summary:
 *    A single star that twinkles
 ************************************************************************/

#pragma once
#include "position.h"   // for POSITION
#include "uiDraw.h"     // for RANDOM and DRAWSTAR

/*****************************************************
 * STAR
 * A star that twinkles
 *****************************************************/
class Star
{
   private:
   Position pos;
   unsigned char phase = random(0, 255); // For out of phase stars
   public:
   void reset(int xMax, int yMax)
   {
      pos.setX(random(0, xMax));
      pos.setY(random(0, yMax));
   }
   void draw(ogstream& gout)
   {
      gout.drawStar(pos, phase);
      phase++;
   }
};
