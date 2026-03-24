#include "simulators/common/scene_render_utils.h"

void runSceneRenderUtilsTests()
{
   ogstream gout;
   const Position viewport(1280.0, 720.0);

   SceneRenderUtils::drawInfoPanel(gout,
                                   Position(20.0, 560.0),
                                   Position(300.0, 700.0),
                                   "Info",
                                   {"Line one", "Line two"},
                                   0.10, 0.12, 0.16);
   SceneRenderUtils::drawLegendPanel(gout,
                                     Position(960.0, 24.0),
                                     Position(1240.0, 180.0),
                                     "Legend",
                                     {
                                        {"Primary", 1.0, 0.8, 0.2},
                                        {"Secondary", 0.4, 0.7, 1.0}
                                     },
                                     0.12, 0.14, 0.18);
   SceneRenderUtils::drawCenteredBanner(gout, viewport, 400.0, "Centered banner");
}
