#include <cassert>
#include <string>

#include "app/scene_status_formatter.h"

void runSceneStatusFormatterTests()
{
   {
      Application app;
      app.openSimulator(SimulatorId::ApolloLander);
      const auto lines = SceneStatusFormatter::format(app);
      assert(lines.size() == 4);
      assert(lines[0].find("Time:") != std::string::npos);
      assert(lines[3].find("Fuel:") != std::string::npos);
   }

   {
      Application app;
      app.openSimulator(SimulatorId::Howitzer);
      const auto lines = SceneStatusFormatter::format(app);
      assert(lines.size() == 4);
      assert(lines[1].find("Angle:") != std::string::npos);
   }

   {
      Application app;
      app.openSimulator(SimulatorId::Orbital);
      const auto lines = SceneStatusFormatter::format(app);
      assert(lines.size() == 4);
      assert(lines[1].find("Live bodies:") != std::string::npos);
   }

   {
      Application app;
      app.openSimulator(SimulatorId::Chess);
      const auto lines = SceneStatusFormatter::format(app);
      assert(lines.size() == 2);
      assert(lines[1].find("White to move") != std::string::npos);
   }
}
