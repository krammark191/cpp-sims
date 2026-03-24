#pragma once

#include "app/preview_storyboard.h"
#include "simulators/lander/legacy/uiDraw.h"

class PreviewStoryboardRenderer
{
public:
   void render(const PreviewStoryboard & storyboard,
               double elapsedSeconds,
               const Position & bottomLeft,
               const Position & topRight,
               ogstream & gout) const;
};
