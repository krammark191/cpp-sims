#include <cstdlib>
#include <iomanip>
#include <iostream>
#if defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#endif

#include "app/application.h"
#include "app/console_runner.h"
#include "app/input_state.h"
#include "app/simulator_registry.h"

int main(int argc, const char * argv[])
{
   Application app;
   const auto & simulators = SimulatorRegistry::all();
   const bool forceInteractive = argc > 1 && std::string(argv[1]) == "--interactive";

#if defined(__unix__) || defined(__APPLE__)
   const bool interactiveTerminal = ::isatty(STDIN_FILENO) != 0;
#else
   const bool interactiveTerminal = false;
#endif

   if (forceInteractive || (interactiveTerminal && argc == 1))
   {
      ConsoleRunner runner;
      return runner.run(app, std::cin, std::cout);
   }

   std::cout << "C++ Simulators\n";
   std::cout << "Initial app state: " << static_cast<int>(app.state()) << "\n";
   std::cout << "Registered simulators: " << simulators.size() << "\n";

   for (const auto & simulator : simulators)
   {
      std::cout << " - " << simulator.displayName << "\n";
   }

   if (app.openSimulator(SimulatorId::ApolloLander))
   {
      InputState input;
      input.mainThrust = true;
      app.setInputState(input);
      app.tick(1.0);
      app.clearInputState();
      std::cout << "Active scene: " << app.activeSceneName() << "\n";
      std::cout << "Status: " << app.activeSceneStatusText() << "\n";
      std::cout << "Scene time: " << std::fixed << std::setprecision(2)
                << app.activeSceneElapsedSeconds() << "s\n";
      std::cout << "Controls:\n";
      for (const auto & binding : app.activeSceneKeybindings())
      {
         std::cout << " - " << binding.action << ": " << binding.primaryKey;
         if (binding.secondaryKey)
            std::cout << " / " << binding.secondaryKey;
         std::cout << "\n";
      }
   }

   return EXIT_SUCCESS;
}
