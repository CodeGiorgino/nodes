#include <filesystem>
#include <format>
#include <print>

#include "enviroment.hxx"
#include "raylib.h"
#include "scene.hxx"

namespace fs = std::filesystem;

static inline constexpr auto initialWidth  = 1920;
static inline constexpr auto initialHeight = 1080;

auto main(int argc, char** argv) -> int {
    fs::path configFilePath { fs::path("assets") / "example.conf" };
    if (argc == 2)
        configFilePath = fs::path{ *(argv + 1) };
    else if (argc > 2)
        throw std::runtime_error(
                    "-- Invalid number of arguments.\n"
                    "-- Execute `nodes --help` for more information.");

    ::SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
    // ::SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    ::InitWindow(initialWidth, initialHeight, "Nodes");
    ::SetTargetFPS(60);

    const auto currentMonitor = ::GetCurrentMonitor();
    ::SetWindowPosition(
            (::GetMonitorWidth(currentMonitor) - initialWidth) * 0.5f,
            (::GetMonitorHeight(currentMonitor) - initialHeight) * 0.5f);

    auto& env = enviroment::get_instance();
    env.init(*argv, configFilePath);

    auto& camera = env.camera();
    camera.zoom = 1.0f;

    scene mainScene { configFilePath };

    // TODO: handle error propagation
    // TODO: snackbar implementation
    while (!::WindowShouldClose()) {
        ::BeginDrawing();
        {
            ::ClearBackground(RAYWHITE);

            mainScene.render();
            mainScene.update();
        }
        ::EndDrawing();
    };

    env.deinit();
    return 0;
}
