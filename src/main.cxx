#include <cmath>
#include <filesystem>
#include <format>
#include <print>
#include <ranges>

#include "enviroment.hxx"
#include "node.hxx"
#include "tokenizer.hxx"
#include "raylib.h"
#include "raymath.h"
#include "uuid.hxx"

namespace fs = std::filesystem;
namespace ranges = std::ranges;
namespace views = std::views;

static constexpr auto initialWidth  = 1920;
static constexpr auto initialHeight = 1080;

auto render_grid(void) noexcept -> void {
    const auto& env = enviroment::get_instance();
    const auto& camera = env.camera();

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
    tokenizer t { fs::path("assets") / "example.conf" };
    for (const auto& token : t.tokens()) {
        // std::println("token: {:?}", token.text);
        std::println("--type: {:?}", tokenizer::token_type_name(token.type));
    }

    return 0;
}

auto _main(int, char** argv) -> int {
    ::SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
    // ::SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    ::InitWindow(initialWidth, initialHeight, "Nodes");
    ::SetTargetFPS(60);

    const auto currentMonitor = ::GetCurrentMonitor();
    ::SetWindowPosition(
            (::GetMonitorWidth(currentMonitor) - initialWidth) * 0.5f,
            (::GetMonitorHeight(currentMonitor) - initialHeight) * 0.5f);

    auto& env = enviroment::get_instance();
    env.init(*argv);

    auto& camera = env.camera();
    camera.zoom = 1.0f;

    std::vector nodes {
        std::make_shared<node>(
            "c79b4316-7236-45ca-ab1b-d5bd8b78a486",
            "this is a title",
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec nec"
            " commodo dui. Mauris eu risus et libero pulvinar facilisis. In"
            " commodo imperdiet mauris, vel hendrerit dolor ultricies quis."
            " Vivamus varius tellus sed libero fringilla ultrices. Maecenas at"
            " ipsum sit amet mauris finibus accumsan. In congue consectetur dui."
            " commodo dui. Mauris eu risus et libero pulvinar facilisis. In"
            " commodo imperdiet mauris, vel hendrerit dolor ultricies quis."
            " Vivamus varius tellus sed libero fringilla ultrices. Maecenas at"
            " ipsum sit amet mauris finibus accumsan. In congue consectetur dui."
            " Nam id fermentum orci.",
            ::Vector2 { 0, 0 }),
        std::make_shared<node>(
            "c79b4316-7236-45ca-ab1b-d5bd8b78a485",
            "this is a title",
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec nec"
            " commodo dui. Mauris eu risus et libero pulvinar facilisis. In"
            " commodo imperdiet mauris, vel hendrerit dolor ultricies quis."
            " Vivamus varius tellus sed libero fringilla ultrices. Maecenas at"
            " ipsum sit amet mauris finibus accumsan. In congue consectetur dui."
            " Nam id fermentum orci.",
            ::Vector2 { 600, 0 }),
        std::make_shared<node>(
            "c79b4316-7236-45ca-ab1b-d5bd8b78a484",
            "this is a title",
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec nec"
            " commodo dui. Mauris eu risus et libero pulvinar facilisis. In"
            " commodo imperdiet mauris, vel hendrerit dolor ultricies quis."
            " Vivamus varius tellus sed libero fringilla ultrices. Maecenas at"
            " ipsum sit amet mauris finibus accumsan. In congue consectetur dui."
            " Nam id fermentum orci.",
            ::Vector2 { 600, 250 }),
    };

    node_map nodeMap {};
    for (const auto& ptr : nodes)
        nodeMap.insert({ std::string(ptr->uuid()), {} });

    while (!::WindowShouldClose()) {
        ::BeginDrawing();
        {
            ::ClearBackground(RAYWHITE);

            ::BeginMode2D(camera);
            {
                render_grid();
                for (const auto& node : nodes)
                    node->render();
            }
            ::EndMode2D();

                for (const auto& node : nodes)
                    node->render_text();

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
                        ::expf(::logf(camera.zoom) + scale),
                        0.5f, 2.75f);

                env.load_font(std::floor(env.fontSizeDefault * camera.zoom));
            }
        }
        ::EndDrawing();
    };

    env.deinit();
    return 0;
}
