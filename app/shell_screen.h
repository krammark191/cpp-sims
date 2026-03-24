#pragma once

#include <string>
#include <vector>

struct ShellScreen
{
   std::string title;
   std::string subtitle;
   std::vector<std::string> bodyLines;
   std::vector<std::string> footerCommands;
};
