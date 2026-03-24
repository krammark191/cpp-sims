#pragma once

#include <iosfwd>

#include "app/application.h"
#include "app/shell_screen.h"

class ShellPresenter
{
public:
   ShellScreen build(const Application & app) const;
   void render(const ShellScreen & screen, std::ostream & output) const;
};
