#pragma once

#include <string>
#include <vector>

struct ShellLayoutLine
{
   double x = 0.0;
   double y = 0.0;
   std::string text;
};

struct ShellLayout
{
   std::vector<ShellLayoutLine> lines;
};
