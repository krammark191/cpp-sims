#include <cassert>
#include <sstream>

#include "app/shell_presenter.h"

void runShellPresenterTests()
{
   ShellPresenter presenter;

   {
      Application app;
      const ShellScreen screen = presenter.build(app);
      assert(screen.title == "C++ Simulators");
      assert(screen.subtitle == "Main Menu");
      assert(!screen.footerCommands.empty());
   }

   {
      Application app;
      app.openPlayMenu();
      const ShellScreen screen = presenter.build(app);
      assert(screen.title == "Play");
      assert(screen.subtitle == "Apollo 11 Lander");
      assert(!screen.bodyLines.empty());

      std::ostringstream output;
      presenter.render(screen, output);
      assert(output.str().find("Preview source:") != std::string::npos);
      assert(output.str().find("> Apollo 11 Lander") != std::string::npos);
   }

   {
      Application app;
      app.openOptions();
      const ShellScreen screen = presenter.build(app);
      assert(screen.title == "Options");

      std::ostringstream output;
      presenter.render(screen, output);
      assert(output.str().find("Audio: hooks ready") != std::string::npos);
      assert(output.str().find("Packaging target: macOS app bundle first") != std::string::npos);
   }

   {
      Application app;
      app.openAbout();
      const ShellScreen screen = presenter.build(app);
      assert(screen.title == "About");

      std::ostringstream output;
      presenter.render(screen, output);
      assert(output.str().find("Apollo 11 Lander") != std::string::npos);
      assert(output.str().find("Mark Van Horn and Taden Marston") != std::string::npos);
      assert(output.str().find("Dr. James Helfrich") != std::string::npos);
      assert(output.str().find("macOS sampler build first") != std::string::npos);
   }

   {
      Application app;
      app.openSimulator(SimulatorId::ApolloLander);
      const ShellScreen screen = presenter.build(app);
      assert(screen.title == "LanderScene");
      assert(!screen.bodyLines.empty());

      std::ostringstream output;
      presenter.render(screen, output);
      assert(output.str().find("Controls overlay") != std::string::npos);
   }

   {
      Application app;
      app.openSimulator(SimulatorId::ApolloLander);
      app.openPauseMenu();
      const ShellScreen screen = presenter.build(app);
      assert(screen.title == "Pause Menu");
      assert(screen.subtitle == "LanderScene");
   }

   {
      Application app;
      app.openSimulator(SimulatorId::ApolloLander);
      app.openPauseMenu();
      app.openKeybindings();
      const ShellScreen screen = presenter.build(app);
      assert(screen.title == "Keybindings");

      std::ostringstream output;
      presenter.render(screen, output);
      assert(output.str().find("Scene controls:") != std::string::npos);
      assert(output.str().find("Resume returns to the paused simulator.") != std::string::npos);
   }
}
