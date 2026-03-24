#pragma once

#include <string>
#include <vector>

namespace ShellTextFormatter
{
std::string formatCommandFooter(const std::vector<std::string> & commands);
std::string formatSelectedCommand(const std::string & command);
}
