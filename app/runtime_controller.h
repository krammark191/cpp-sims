#pragma once

#include <string>

#include "app/application.h"
#include "app/runtime_command_result.h"

class RuntimeController
{
public:
   RuntimeCommandResult handleCommand(Application & app,
                                     const std::string & command) const;
};
