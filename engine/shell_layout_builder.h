#pragma once

#include "app/shell_screen.h"
#include "engine/shell_layout.h"

class ShellLayoutBuilder
{
public:
   ShellLayout build(const ShellScreen & screen,
                     double viewportWidth,
                     double viewportHeight) const;
};
