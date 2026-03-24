cmake_minimum_required(VERSION 3.24)

project(CPPSimulators LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

   add_library(sampler_app
      app/application.cpp
      app/console_runner.cpp
      app/control_binding_formatter.cpp
      app/engine_shell_session.cpp
      app/preview_storyboard.cpp
      app/runtime_paths.cpp
      app/runtime_controller.cpp
      app/scene_input_router.cpp
      app/scene_status_formatter.cpp
      app/shell_command_catalog.cpp
      app/shell_presenter.cpp
      app/shell_text_formatter.cpp
      engine/engine_shell_renderer.cpp
      engine/image_sequence.cpp
      engine/preview_storyboard_renderer.cpp
      engine/shell_layout.cpp
   app/scene_factory.cpp
   app/simulator_registry.cpp
   simulators/chess/chess_legacy_amalgam.cpp
   simulators/chess/chess_scene.cpp
   simulators/common/placeholder_scene.cpp
    simulators/common/scene_render_utils.cpp
   simulators/howitzer/howitzer_legacy_amalgam.cpp
   simulators/howitzer/howitzer_scene.cpp
   simulators/lander/legacy/acceleration.cpp
   simulators/lander/legacy/angle.cpp
   simulators/lander/legacy/ground.cpp
   simulators/lander/legacy/lander.cpp
   simulators/lander/legacy/legacy_draw_stub.cpp
   simulators/lander/legacy/position.cpp
   simulators/lander/legacy/velocity.cpp
   simulators/lander/lander_scene.cpp
   simulators/orbital/orbital_legacy_amalgam.cpp
   simulators/orbital/orbital_scene.cpp
)

target_include_directories(sampler_app
   PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(cpp_simulators
   "C++ Simulators/main.cpp"
)

target_link_libraries(cpp_simulators
   PRIVATE
      sampler_app
)

if(APPLE OR WIN32)
   set(CPP_SIMULATORS_WINDOW_SOURCES
      "C++ Simulators/window_main.cpp"
      app/window_shell_runner.cpp
      app/application.cpp
      app/control_binding_formatter.cpp
      app/engine_shell_session.cpp
      app/preview_storyboard.cpp
      app/runtime_paths.cpp
      app/runtime_controller.cpp
      app/scene_input_router.cpp
      app/scene_status_formatter.cpp
      app/shell_command_catalog.cpp
      app/shell_presenter.cpp
      app/shell_text_formatter.cpp
      engine/engine_shell_renderer.cpp
      engine/image_sequence.cpp
      engine/preview_storyboard_renderer.cpp
      engine/shell_layout.cpp
      app/scene_factory.cpp
      app/simulator_registry.cpp
      simulators/chess/chess_legacy_gui_amalgam.cpp
      simulators/chess/chess_scene.cpp
      simulators/common/placeholder_scene.cpp
      simulators/common/scene_render_utils.cpp
      simulators/howitzer/howitzer_legacy_gui_amalgam.cpp
      simulators/howitzer/howitzer_scene.cpp
      simulators/lander/legacy/acceleration.cpp
      simulators/lander/legacy/angle.cpp
      simulators/lander/legacy/ground.cpp
      simulators/lander/legacy/lander.cpp
      simulators/lander/legacy/position.cpp
      simulators/lander/legacy/uiDraw.cpp
      simulators/lander/legacy/uiInteract.cpp
      simulators/lander/legacy/velocity.cpp
      simulators/lander/lander_scene.cpp
      simulators/orbital/orbital_legacy_gui_amalgam.cpp
      simulators/orbital/orbital_scene.cpp
   )

   if(WIN32)
      add_executable(cpp_simulators_window WIN32
         ${CPP_SIMULATORS_WINDOW_SOURCES}
      )
   else()
   add_executable(cpp_simulators_window
         ${CPP_SIMULATORS_WINDOW_SOURCES}
      )
   endif()

   target_include_directories(cpp_simulators_window
      PRIVATE
         ${CMAKE_CURRENT_SOURCE_DIR}
   )

   target_compile_definitions(cpp_simulators_window
      PRIVATE
         CPP_SIMULATORS_WINDOW
   )

   if(APPLE)
      target_link_libraries(cpp_simulators_window
         PRIVATE
            "-framework ApplicationServices"
            "-framework OpenGL"
            "-framework GLUT"
      )
   elseif(WIN32)
      find_package(OpenGL REQUIRED)
      find_package(GLUT REQUIRED)

      target_compile_definitions(cpp_simulators_window
         PRIVATE
            WIN32_LEAN_AND_MEAN
            NOMINMAX
      )

      target_link_libraries(cpp_simulators_window
         PRIVATE
            OpenGL::GL
            GLUT::GLUT
            glu32
            windowscodecs
            ole32
      )
   endif()
endif()

if(APPLE)
   set_target_properties(cpp_simulators PROPERTIES
      MACOSX_BUNDLE TRUE
      MACOSX_BUNDLE_BUNDLE_NAME "C++ Simulators"
   )
endif()

include(CTest)

if(BUILD_TESTING)
   add_executable(sampler_app_tests
      tests/unit/test_main.cpp
      tests/unit/application_tests.cpp
      tests/unit/control_binding_formatter_tests.cpp
      tests/unit/chess_scene_tests.cpp
      tests/unit/howitzer_scene_tests.cpp
      tests/unit/lander_scene_tests.cpp
      tests/unit/orbital_scene_tests.cpp
      tests/unit/preview_storyboard_tests.cpp
      tests/unit/scene_render_utils_tests.cpp
      tests/unit/engine_shell_session_tests.cpp
      tests/unit/runtime_controller_tests.cpp
      tests/unit/scene_input_router_tests.cpp
      tests/unit/scene_status_formatter_tests.cpp
      tests/unit/shell_command_catalog_tests.cpp
      tests/unit/shell_presenter_tests.cpp
      tests/unit/shell_text_formatter_tests.cpp
      tests/unit/simulator_registry_tests.cpp
   )

   target_link_libraries(sampler_app_tests
      PRIVATE
         sampler_app
   )

   add_executable(chess_rules_tests
      tests/chess/chess_rules_tests.cpp
      simulators/chess/legacy/board.cpp
      simulators/chess/legacy/move.cpp
      simulators/chess/legacy/piece.cpp
      simulators/chess/legacy/pieceBishop.cpp
      simulators/chess/legacy/pieceKing.cpp
      simulators/chess/legacy/pieceKnight.cpp
      simulators/chess/legacy/piecePawn.cpp
      simulators/chess/legacy/pieceQueen.cpp
      simulators/chess/legacy/pieceRook.cpp
      simulators/chess/legacy/position.cpp
      simulators/chess/legacy/legacy_draw_stub.cpp
   )

   target_include_directories(chess_rules_tests
      PRIVATE
         ${CMAKE_CURRENT_SOURCE_DIR}
   )

   add_test(NAME sampler_app_tests COMMAND sampler_app_tests)
   add_test(NAME chess_rules_tests COMMAND chess_rules_tests)
endif()
