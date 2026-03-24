#include "engine/engine_shell_renderer.h"

ShellLayout EngineShellRenderer::compose(const ShellScreen & screen,
                                         const Position & viewport) const
{
   return layoutBuilder.build(screen, viewport.getX(), viewport.getY());
}

void EngineShellRenderer::render(const ShellLayout & layout, ogstream & gout) const
{
   for (const auto & line : layout.lines)
   {
      gout.setPosition(Position(line.x, line.y));
      gout << line.text;
   }
}
