#include "enviroment.hxx"
#include "gui.hxx"
#include "node.hxx"
#include "raylib.h"
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
    const auto titleSize = gui::draw_text_rec(_title.wrapped, _pos, width,
            padding, ::ColorAlpha(::BLUE, 0.2f));

    const ::Vector2 descriptionPos {
        _pos.x,
        _pos.y + titleSize.y
    };

    gui::draw_text_rec(_description.wrapped, descriptionPos, width);
}
