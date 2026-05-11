#include <chrono>

#include "enviroment.hxx"
#include "gui.hxx"
#include "node.hxx"
#include "raylib.h"
#include "raymath.h"
#include "uuid.hxx"

namespace chrono = std::chrono;

node::node_error::node_error(std::string_view message) noexcept {
    _message += message;
}

auto node::node_error::what(void) const noexcept -> const char* {
    return _message.c_str();
}

node::node(std::string_view uuid, std::string_view title,
        std::string_view description, ::Vector2 position) :
    _uuid(uuid),
    _pos(std::move(position)) {
        if (uuid.empty())
            throw node::node_error(
                    "Cannot set the uuid to be empty.");
        else if (!uuid::test_v4(uuid))
            throw node::node_error(
                    std::format(
                        "Invalid uuid v4 provided: {:?}.",
                        uuid));

        const float maxWidth = node::style::width - node::style::padding * 4.0f;

        { // title
            _title = node_text {
                std::string { title },
                gui::wrap_text(title, maxWidth),
            };

            const auto textSize = gui::measure_text(_title.wrapped);
            _titleSize = ::Vector2 {
                node::style::width,
                textSize.y + node::style::padding * 2.0f,
            };
        }

        { // description
            _description = node_text {
                std::string { description },
                gui::wrap_text(description, maxWidth),
            };

            const auto textSize = gui::measure_text(_description.wrapped);
            _descriptionSize = ::Vector2 {
                node::style::width,
                textSize.y + node::style::padding * 3.0f,
            };
        }

        _size = {
            _titleSize.x,
            _titleSize.y + _descriptionSize.y,
        };
    }

auto node::create(std::string_view uuid, std::string_view title,
        std::string_view description, ::Vector2 position) -> shared_ptr {
    return std::make_shared<node>(node {
                std::forward<std::string_view>(uuid),
                std::forward<std::string_view>(title),
                std::forward<std::string_view>(description),
                std::forward<::Vector2>(position),
            });
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
    return _size;
}

auto node::check_collision(void) const noexcept -> bool {
    const auto& env = enviroment::get_instance();
    const auto& camera = env.camera();

    const auto mouseWorldPos =
        ::GetScreenToWorld2D(::GetMousePosition(), camera);

    return ::CheckCollisionPointRec(mouseWorldPos, {
                _pos.x,
                _pos.y,
                _size.x,
                _size.y,
            });
}

auto node::update(void) -> void {
    // TODO: update node
}

auto node::render(void) const -> void {
    const auto& env = enviroment::get_instance();
    const auto& camera = env.camera();
    const auto screenPos = ::GetWorldToScreen2D(_pos, camera);

    // title background
    ::BeginMode2D(camera);
    {
        ::BeginScissorMode(screenPos.x, screenPos.y, _titleSize.x * camera.zoom,
                _titleSize.y * camera.zoom);
        {
            ::DrawRectangleRounded({
                        _pos.x,
                        _pos.y,
                        _titleSize.x,
                        _titleSize.y + _descriptionSize.y,
                    }, node::style::borderRoundness, node::style::borderSegments,
                    node::style::highligthColor);
        }
        ::EndScissorMode();

        // description background
        ::DrawRectangleV({
                    _pos.x,
                    _pos.y + _titleSize.y,
                }, _descriptionSize, node::style::backgroundColor);

        // horizontal separator
        ::DrawLineEx({
                    _pos.x,
                    _pos.y + _titleSize.y,
                }, {
                    _pos.x + _titleSize.x,
                    _pos.y + _titleSize.y
                }, node::style::borderThickness, node::style::borderColor);

        // border
        ::DrawRectangleRoundedLinesEx({
                _pos.x,
                _pos.y,
                _titleSize.x,
                _titleSize.y + _descriptionSize.y,
            }, node::style::borderRoundness, node::style::borderSegments,
            node::style::borderThickness, node::style::borderColor);
    }
    ::EndMode2D();

    render_text();
}

auto node::render_text(void) const -> void {
    const auto& env = enviroment::get_instance();
    const auto& camera = env.camera();

    ::Vector2 textPos {
        _pos.x + node::style::padding,
        _pos.y + node::style::padding,
    };

    gui::draw_text(_title.wrapped, ::GetWorldToScreen2D(textPos, camera),
            node::style::textColor);

    const auto titleSize = gui::measure_text(_title.wrapped);
    textPos.y += titleSize.y + node::style::padding * 2.0f;

    gui::draw_text(_description.wrapped, ::GetWorldToScreen2D(textPos, camera),
            node::style::textColor);
}
