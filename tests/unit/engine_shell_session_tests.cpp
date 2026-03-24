#include <cassert>

#include "app/engine_shell_session.h"

void runEngineShellSessionTests()
{
   EngineShellSession session;
   Position viewport(1280.0, 720.0);

   {
      Application app;
      const ShellLayout layout = session.compose(app, viewport);
      assert(!layout.lines.empty());
      assert(layout.lines.front().text == "C++ Simulators");
   }

   {
      Application app;
      app.openSimulator(SimulatorId::ApolloLander);
      const ShellLayout layout = session.compose(app, viewport);
      assert(layout.lines.size() >= 6);
      bool foundOverlay = false;
      for (const auto & line : layout.lines)
      {
         if (line.text == "Controls overlay")
            foundOverlay = true;
      }
      assert(foundOverlay);
   }

   {
      Application app;
      app.openSimulator(SimulatorId::ApolloLander);
      app.openPauseMenu();
      const ShellLayout layout = session.compose(app, viewport);
      assert(!layout.lines.empty());
      assert(layout.lines.front().text == "Pause Menu");
   }
}
