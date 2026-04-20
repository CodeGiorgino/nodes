#include <cmath>
#include <format>
#include <ranges>

#include "enviroment.hxx"
#include "raylib.h"
#include "raymath.h"

namespace ranges = std::ranges;
namespace views = std::views;

static constexpr auto initialWidth  = 1920;
static constexpr auto initialHeight = 1080;

template<std::integral T>
constexpr auto nearestMultiplier(T a, T b) -> T {
    if (a == 0) return 0;
    return a + (b - (a % b));
}

auto render_grid(void) noexcept -> void {
    const auto& env = enviroment::get_instance();
    const auto& camera = env.get_camera();

    const auto topLeft = ::GetScreenToWorld2D({ 0, 0 }, camera);
    const auto bottomRight = ::GetScreenToWorld2D({
                (float)::GetScreenWidth(),
                (float)::GetScreenHeight()
            }, camera);

    const auto gridStart = ::Vector2 {
        std::floor(topLeft.x / env.gridSize) * env.gridSize,
        std::floor(topLeft.y / env.gridSize) * env.gridSize,
    };

    const auto gridEnd = ::Vector2 {
        std::ceil(bottomRight.x / env.gridSize) * env.gridSize,
        std::ceil(bottomRight.y / env.gridSize) * env.gridSize,
    };

    for (float y : views::iota(0)
            | views::take((int)((gridEnd.y - gridStart.y) / env.gridSize) + 1)) {
        const auto yPos = gridStart.y + (y * env.gridSize);
        ::DrawLine(gridStart.x, yPos, gridEnd.x, yPos, ::LIGHTGRAY);
    }

    for (float x : views::iota(0)
            | views::take((int)((gridEnd.x - gridStart.x) / env.gridSize) + 1)) {
        const auto xPos = gridStart.x + (x * env.gridSize);
        ::DrawLine(xPos, gridStart.y, xPos, gridEnd.y, ::LIGHTGRAY);
    }
}

auto main(void) -> int {
    ::SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT
            | FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
    ::InitWindow(initialWidth, initialHeight, "House plan maker");
    ::SetTargetFPS(60);
    ::SetExitKey(KEY_Q);

    const auto currentMonitor = ::GetCurrentMonitor();
    ::SetWindowPosition(
            (::GetMonitorWidth(currentMonitor) - initialWidth) * 0.5f,
            (::GetMonitorHeight(currentMonitor) - initialHeight) * 0.5f);

    auto& env = enviroment::get_instance();
    auto& camera = env.get_camera();
    camera.zoom = 1.0f;

    while (!::WindowShouldClose()) {
        ::BeginDrawing();
        {
            ::ClearBackground(RAYWHITE);

            ::BeginMode2D(camera);
            {
                render_grid();
            }
            ::EndMode2D();

            // update the camera position
            if (::IsMouseButtonDown(::MOUSE_BUTTON_LEFT)) {
                auto delta = ::GetMouseDelta();
                delta = ::Vector2Scale(delta, -1.0f / camera.zoom);
                camera.target = ::Vector2Add(camera.target, delta);
            }

            // update the camera zoom
            const auto wheel = ::GetMouseWheelMove();
            if (wheel != 0) {
                const auto mouseWorldPosition =
                    ::GetScreenToWorld2D(::GetMousePosition(), camera);
                camera.offset = ::GetMousePosition();
                camera.target = mouseWorldPosition;

                const auto scale = wheel * 0.25f;
                camera.zoom = std::clamp(
                        ::expf(::logf(camera.zoom) + scale), 0.05f, 64.0f);
            }
        }
        ::EndDrawing();
    };

    return 0;
}
