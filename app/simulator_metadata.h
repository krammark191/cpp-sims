#pragma once

#include <string>

#include "app/simulator_id.h"

struct SimulatorMetadata
{
   SimulatorId id;
   const char * displayName;
   const char * summary;
   const char * demoVideoPath;
   const char * previewManifestPath;
   const char * sceneClassName;
   bool supportsRecordedPreview;
   bool supportsFutureAiModes;
};
