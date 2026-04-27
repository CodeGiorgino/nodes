#include "enviroment.hxx"
#include "gui.hxx"
#include "node.hxx"
#include "raylib.h"
#include "raymath.h"
#include "uuid.hxx"

node::node(void)
    noexcept : _uuid(uuid::generate_v4()) {
        // empty
    }

node::node(std::string_view uuid, std::string_view title,
        std::string_view description, ::Vector2 pos)
    : _uuid(uuid), _pos(std::move(pos)) {
        if (uuid.empty())
            throw std::runtime_error(
                    "node error - cannot set the uuid to be empty");
        else if (!uuid::test_v4(uuid))
            throw std::runtime_error(
                    std::format(
                        "node error - invalid uuid v4 provided: {:?}", uuid));

        const float maxWidth = width - padding * 2;
        _title = node_text {
            .text = std::string { title },
            .wrapped = gui::wrap_text(title, maxWidth),
        };

        _description = node_text {
            .text = std::string { description },
            .wrapped = gui::wrap_text(description, maxWidth),
        };
    }

auto node::update(void) noexcept -> void {
    // TODO: update node
}

auto node::render(void) const noexcept -> void {
    const auto& env = enviroment::get_instance();
    const auto& camera = env.camera();

    auto recPos = _pos;

    { // draw title
        const auto textSize = gui::measure_text(_title.wrapped);
        const ::Vector2 recSize {
            width + padding / camera.zoom,
            textSize.y / camera.zoom + padding * 2.0f,
        };

        ::DrawRectangleV(recPos, recSize, ::ColorAlpha(::BLUE, 0.2));
        ::DrawRectangleLines(recPos.x, recPos.y, recSize.x, recSize.y, ::BLACK);

        recPos.y += recSize.y;
    }

    { // draw description
        const auto textSize = gui::measure_text(_description.wrapped);
        const ::Vector2 recSize {
            width + padding / camera.zoom,
            textSize.y / camera.zoom + padding * 2.0f,
        };

        ::DrawRectangleV(recPos, recSize, ::ColorAlpha(::LIGHTGRAY, 0.2));
        ::DrawRectangleLines(recPos.x, recPos.y, recSize.x, recSize.y, ::BLACK);
    }
}

auto node::render_text(void) const noexcept -> void {
    const auto& env = enviroment::get_instance();
    const auto& camera = env.camera();

    ::Vector2 textPos {
        _pos.x + padding,
        _pos.y + padding,
    };

    gui::draw_text(_title.wrapped, ::GetWorldToScreen2D(textPos, camera));

    const auto titleSize = gui::measure_text(_title.wrapped);
    textPos.y += titleSize.y / camera.zoom + padding * 2.0f;

    gui::draw_text(_description.wrapped, ::GetWorldToScreen2D(textPos, camera));
}
