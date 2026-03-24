#include "app/engine_shell_session.h"

ShellLayout EngineShellSession::compose(const Application & app, const Position & viewport) const
{
   return renderer.compose(presenter.build(app), viewport);
}

void EngineShellSession::render(const Application & app,
                                const Position & viewport,
                                ogstream & gout) const
{
   renderer.render(compose(app, viewport), gout);
}
