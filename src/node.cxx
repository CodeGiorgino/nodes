#include "enviroment.hxx"
#include "gui.hxx"
#include "node.hxx"
#include "raylib.h"
#include "raymath.h"
#include "uuid.hxx"

node::node_error::node_error(std::string_view message) noexcept {
    _message += message;
}

auto node::node_error::what(void) const noexcept -> const char* {
    return _message.c_str();
}

node::node(void)
    noexcept : _uuid(uuid::generate_v4()) {
        // empty
    }

node::node(std::string_view uuid, std::string_view title,
        std::string_view description, ::Vector2 pos)
    : _uuid(uuid), _pos(std::move(pos)) {
        if (uuid.empty())
            throw node::node_error(
                    "Cannot set the uuid to be empty.");
        else if (!uuid::test_v4(uuid))
            throw node::node_error(
                    std::format(
                        "Invalid uuid v4 provided: {:?}.",
                        uuid));

        const float maxWidth = width - padding * 4.0f;
        _title = node_text {
            .text = std::string { title },
            .wrapped = gui::wrap_text(title, maxWidth),
        };

        _description = node_text {
            .text = std::string { description },
            .wrapped = gui::wrap_text(description, maxWidth),
        };
    }

auto node::uuid(void) const noexcept -> std::string {
    return _uuid;
}

auto node::update(void) noexcept -> void {
    // TODO: update node
}

auto node::render(void) const noexcept -> void {
    auto recPos = _pos;

    { // draw title
        const auto textSize = gui::measure_text(_title.wrapped);
        const ::Vector2 recSize {
            width,
            textSize.y + padding * 2.0f,
        };

        ::DrawRectangleV(recPos, recSize, ::ColorAlpha(::BLUE, 0.2));
        ::DrawRectangleLines(recPos.x, recPos.y, recSize.x, recSize.y, ::BLACK);

        recPos.y += recSize.y;
    }

    { // draw description
        const auto textSize = gui::measure_text(_description.wrapped);
        const ::Vector2 recSize {
            width,
            textSize.y + padding * 2.0f,
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
    textPos.y += titleSize.y + padding * 2.0f;

    gui::draw_text(_description.wrapped, ::GetWorldToScreen2D(textPos, camera));
}
