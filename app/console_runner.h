#pragma once

#include <iosfwd>

#include "app/application.h"
#include "app/runtime_controller.h"
#include "app/shell_presenter.h"

class ConsoleRunner
{
public:
   int run(Application & app, std::istream & input, std::ostream & output);

private:
   RuntimeController controller;
   ShellPresenter presenter;
};
