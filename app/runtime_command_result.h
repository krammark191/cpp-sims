#pragma once

#include <string>
#include <vector>

struct RuntimeCommandResult
{
   bool continueRunning = true;
   bool renderScreen = true;
   std::vector<std::string> notices;
};
