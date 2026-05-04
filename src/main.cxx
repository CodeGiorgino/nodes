#include <cmath>
#include <filesystem>
#include <format>
#include <print>
#include <ranges>
#include <unordered_map>

#include "enviroment.hxx"
#include "lexer.hxx"
#include "node.hxx"
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

auto main(int argc, char** argv) -> int {
    fs::path configFilePath { fs::path("assets") / "example.conf" };
    if (argc == 2)
        configFilePath = fs::path{ *(argv + 1) };
    else if (argc > 2)
        throw std::runtime_error(
                    "Invalid number of arguments.\n"
                    "Execute `nodes --help` for more information.");

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

    lexer lex { configFilePath };
    auto nodes = lex.nodes() | ranges::to<std::vector>();

    // get the connections to each node
    std::unordered_map<std::string, std::vector<node_ptr>> nodeMap {};
    for (const auto& node : nodes)
        for (const auto& uuid : node->connections())
            if (auto it = nodeMap.find(uuid);
                    it != nodeMap.end())
                (*it).second.push_back(node);
            else nodeMap.insert({ uuid, { node } });

    while (!::WindowShouldClose()) {
        ::BeginDrawing();
        {
            ::ClearBackground(RAYWHITE);

            ::BeginMode2D(camera);
            {
                render_grid();

                for (const auto& node : nodes) {
                    node->render();

                    // no connections to the node
                    if (!nodeMap.contains(node->uuid()))
                        continue;

                    // render the connections
                    const auto& pos = node->position();

                    // TODO: order the connections based on the y position
                    // of the node to which to connect to, then draw the
                    // connections using the formula:
                    //
                    // y = nodeHeight / 2.0f + gap (index - connectionIndex)
                    //
                    // Use the formula to get the relative y position (distance
                    // from the top of the node position) of both the origin
                    // node and the destination node

                    for (const auto& uuid : node->connections()) {
                        // unreachable: node map creation error
                        if (!nodeMap.contains(uuid))
                            throw std::runtime_error(
                                    std::format(
                                        "Missing connection definitions in node map: {:?}",
                                        uuid));
                    }
                }
            }
            ::EndMode2D();

            for (const auto& node : nodes) {
                node->render_text();
                node->update();
            }

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
