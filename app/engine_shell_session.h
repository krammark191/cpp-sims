#pragma once

#include "app/application.h"
#include "app/shell_presenter.h"
#include "engine/engine_shell_renderer.h"

class EngineShellSession
{
public:
   ShellLayout compose(const Application & app, const Position & viewport) const;
   void render(const Application & app, const Position & viewport, ogstream & gout) const;

private:
   ShellPresenter presenter;
   EngineShellRenderer renderer;
};
