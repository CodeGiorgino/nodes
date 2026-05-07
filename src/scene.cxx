#include <cmath>
#include <ranges>

#include "enviroment.hxx"
#include "lexer.hxx"
#include "raylib.h"
#include "raymath.h"
#include "scene.hxx"

namespace fs = std::filesystem;
namespace ranges = std::ranges;
namespace views = std::views;

scene::scene_error::scene_error(std::string_view message) noexcept {
    _message += message;
}

auto scene::scene_error::what(void) const noexcept -> const char* {
    return _message.c_str();
}

scene::scene(fs::path configFilePath) {
    load(configFilePath);
}

auto scene::load(void) -> void {
    if (_configFilePath.string().empty())
        throw scene::scene_error(
                "Cannot load scene: no config file path has been provided.");
    load(_configFilePath);
}

auto scene::load(fs::path configFilePath) -> void {
    _configFilePath = std::move(configFilePath);
    lexer lex { _configFilePath };

    _nodes = lex.nodes()
        | ranges::to<std::vector>();

    // get the connections to each node
    _nodeMap.clear();
    for (const auto& node : _nodes)
        for (const auto& uuid : node->connections())
            if (auto it = _nodeMap.find(uuid);
                    it != _nodeMap.end())
                it->second.push_back(node);
            else _nodeMap.insert({ uuid, { node } });
}

auto scene::render_grid(void) const noexcept -> void {
    const auto& env = enviroment::get_instance();
    const auto& camera = env.camera();

    const auto topLeft = ::GetScreenToWorld2D({ 0, 0 }, camera);
    const auto bottomRight = ::GetScreenToWorld2D({
                (float)::GetScreenWidth(),
                (float)::GetScreenHeight(),
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
        ::DrawLine(gridStart.x, yPos, gridEnd.x, yPos, scene::style::gridColor);
    }

    for (float x : views::iota(0)
            | views::take((int)((gridEnd.x - gridStart.x) / env.gridSize) + 1)) {
        const auto xPos = gridStart.x + (x * env.gridSize);
        ::DrawLine(xPos, gridStart.y, xPos, gridEnd.y, scene::style::gridColor);
    }
}

auto scene::render_nodes(void) const -> void {
    for (const auto& node : _nodes) {
        // iterate connections
        for (size_t indexFrom = 0; indexFrom < node->connections().size();
                indexFrom++) {
            const auto& connectedNodeUuid =
                node->connections()[indexFrom];
            const auto& connectedNode =
                [&](void) -> node_ptr {
                    for (const auto& n : _nodes)
                        if (n->uuid() == connectedNodeUuid)
                            return n;
                    throw std::runtime_error(
                            std::format(
                                "-- Configuration error - Cannot find node {:?} connected to {:?}",
                                connectedNodeUuid, node->uuid()));
                }();

            // find connections definition to the connected node
            const auto it = _nodeMap.find(connectedNodeUuid);
            if (it == _nodeMap.end())
                // unreachable: node map creation error
                throw std::runtime_error(
                        std::format(
                            "-- Node map error - Missing connection definitions in node map: {:?}",
                            connectedNodeUuid));

            // find the node index in the connected node connections
            const auto indexTo =
                [&](void) -> size_t {
                    for (size_t i = 0; it->second.size(); i++)
                        if (it->second[i]->uuid() == node->uuid())
                            return i;
                    throw std::runtime_error(
                            std::format(
                                "-- Node map error - Missing connection definition from node {:?} to node {:?}",
                                node->uuid(), connectedNodeUuid));
                }();

            // draw the connection based on the formula:
            // y = nodeHeight / 2.0f + gap (connectionIndex - 1)

            const ::Vector2 startPos {
                node->position().x + node->size().x,
                    node->position().y
                        + node->size().y * 0.5f
                        + node::style::connectionGap * ((int)indexFrom - 1),
            };

            const ::Vector2 endPos {
                connectedNode->position().x,
                    connectedNode->position().y
                        + connectedNode->size().y * 0.5f
                        + node::style::connectionGap * ((int)indexTo - 1),
            };

            ::DrawLineBezier(startPos, endPos, node::style::connectionThickness,
                    node::style::connectionColor);
        }

        node->render();
    }
}

auto scene::render(void) const -> void {
    const auto& env = enviroment::get_instance();
    const auto& camera = env.camera();

    ::ClearBackground(scene::style::backgroundColor);

    ::BeginMode2D(camera);
    {
        render_grid();
        render_nodes();
    }
    ::EndMode2D();

    for (const auto& node : _nodes) {
        node->render_text();
    }
}

auto scene::update(void) -> void {
    auto& env = enviroment::get_instance();
    auto& camera = env.camera();

    // update the camera position
    if (::IsMouseButtonDown(::MOUSE_BUTTON_LEFT)) {
        const auto delta = ::Vector2Scale(::GetMouseDelta(),
                -1.0f / camera.zoom);
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
                ::expf(::logf(camera.zoom) + scale), 0.5f, 2.75f);

        env.load_font(std::floor(env.fontSizeDefault * camera.zoom));
    }

    for (const auto& node : _nodes) {
        node->update();
    }
}
