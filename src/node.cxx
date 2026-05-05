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

        { // title
            _title = node_text {
                std::string { title },
                gui::wrap_text(title, maxWidth),
            };

            const auto textSize = gui::measure_text(_title.wrapped);
            _titleSize = ::Vector2 {
                width,
                textSize.y + padding * 2.0f,
            };
        }

        { // description
            _description = node_text {
                std::string { description },
                gui::wrap_text(description, maxWidth),
            };

            const auto textSize = gui::measure_text(_description.wrapped);
            _descriptionSize = ::Vector2 {
                width,
                textSize.y + padding * 2.0f,
            };
        }
    }

auto node::uuid(void) const noexcept -> std::string {
    return _uuid;
}

auto node::title_size(void) const noexcept -> ::Vector2 {
    return _titleSize;
}

auto node::description_size(void) const noexcept -> ::Vector2 {
    return _descriptionSize;
}

auto node::size(void) const noexcept -> ::Vector2 {
    return {
        _titleSize.x,
        _titleSize.y + _descriptionSize.y
    };
}

auto node::update(void) noexcept -> void {
    // TODO: update node
}

auto node::render(void) const noexcept -> void {
    auto recPos = _pos;

    { // draw title
        ::DrawRectangleV(recPos, _titleSize, ::ColorAlpha(::BLUE, 0.2));
        ::DrawRectangleLines(recPos.x, recPos.y, _titleSize.x, _titleSize.y,
                ::BLACK);

        recPos.y += _titleSize.y;
    }

    { // draw description
        ::DrawRectangleV(recPos, _descriptionSize, ::ColorAlpha(::LIGHTGRAY, 0.2));
        ::DrawRectangleLines(recPos.x, recPos.y, _descriptionSize.x,
                _descriptionSize.y, ::BLACK);
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
