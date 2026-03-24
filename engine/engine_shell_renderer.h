#pragma once

#include "app/shell_screen.h"
#include "engine/shell_layout_builder.h"
#include "simulators/lander/legacy/position.h"
#include "simulators/lander/legacy/uiDraw.h"

class EngineShellRenderer
{
public:
   ShellLayout compose(const ShellScreen & screen, const Position & viewport) const;
   void render(const ShellLayout & layout, ogstream & gout) const;

private:
   ShellLayoutBuilder layoutBuilder;
};
