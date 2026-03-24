#include <cassert>

#include "app/preview_storyboard.h"
#include "app/simulator_registry.h"
#include "engine/preview_storyboard_renderer.h"

void runPreviewStoryboardTests()
{
   const auto * apollo = SimulatorRegistry::find(SimulatorId::ApolloLander);
   assert(apollo != nullptr);
   assert(PreviewStoryboardLoader::hasStoryboard(*apollo));

   const auto storyboard = PreviewStoryboardLoader::load(*apollo);
   assert(storyboard.has_value());
   assert(storyboard->title == "Apollo 11 Lander");
   assert(storyboard->frames.size() == 3);
   assert(storyboard->frameSeconds > 2.0);
   assert(storyboard->frames.front().image.has_value());
   assert(storyboard->frames.front().image->width == 8);
   assert(storyboard->frames.front().image->height == 6);
   assert(PreviewStoryboardLoader::frameIndexAtTime(*storyboard, 0.1) == 0);
   assert(PreviewStoryboardLoader::frameIndexAtTime(*storyboard, storyboard->frameSeconds + 0.1) == 1);

   PreviewStoryboardRenderer renderer;
   ogstream gout;
   renderer.render(*storyboard, 1.0, Position(100.0, 100.0), Position(900.0, 500.0), gout);
}
