#include <cassert>
#include <string>

#include "app/input_state.h"
#include "simulators/chess/chess_scene.h"

void runChessSceneTests()
{
   {
      ChessScene scene;

      scene.reset();
      scene.enter();

      assert(scene.id() == SimulatorId::Chess);
      assert(scene.isReady());
      assert(!scene.isPlaceholder());
      assert(scene.supportsRecordedPreview());
      assert(scene.supportsFutureAiModes());
      assert(!scene.hasPendingPromotion());
      assert(scene.elapsedSeconds() == 0.0);
      assert(std::string(scene.statusText()).find("White to move") != std::string::npos);
   }

   {
      ChessScene scene;
      InputState input;

      scene.reset();
      scene.enter();

      input.firePressed = true;
      input.selectPressed = true;
      scene.applyInput(input);
      scene.update(0.1);

      scene.applyInput(InputState{});
      scene.update(0.1);

      input = InputState{};
      input.upPressed = true;
      scene.applyInput(input);
      scene.update(0.1);

      scene.applyInput(InputState{});
      scene.update(0.1);

      input = InputState{};
      input.firePressed = true;
      input.selectPressed = true;
      scene.applyInput(input);
      scene.update(0.1);

      assert(scene.elapsedSeconds() > 0.0);
      assert(std::string(scene.statusText()).find("Black to move") != std::string::npos);
   }

   {
      ChessScene scene;
      ogstream gout;

      scene.reset();
      scene.enter();
      scene.render(gout, Position(1280.0, 720.0));
   }
}
