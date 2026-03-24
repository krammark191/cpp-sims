#include "app/console_runner.h"

#include <istream>
#include <ostream>
#include <string>

int ConsoleRunner::run(Application & app, std::istream & input, std::ostream & output)
{
   presenter.render(presenter.build(app), output);

   std::string command;
   while (std::getline(input, command))
   {
      const RuntimeCommandResult result = controller.handleCommand(app, command);

      for (const auto & notice : result.notices)
         output << notice << "\n";

      if (!result.continueRunning)
         return 0;

      if (result.renderScreen)
         presenter.render(presenter.build(app), output);
   }

   return 0;
}
