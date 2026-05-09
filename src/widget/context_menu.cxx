#include <cmath>

#include "gui.hxx"
#include "raymath.h"
#include "widget/context_menu.hxx"

namespace chrono = std::chrono;

namespace widget {
    context_menu::context_menu(::Rectangle rec,
            std::initializer_list<context_menu::item> items,
            context_menu::options opts) noexcept :
        base<context_menu>(std::forward<::Rectangle>(rec)),
        _opts(std::move(opts)) {
            // set initial size
            if (_opts.fitSize)
                _size = { 0, context_menu::style::padding * 2.0f };

            // calculate size based on items
            for (const auto& [text, e] : items) {
                _items.emplace_back(text, e);
                if (_opts.fitSize) {
                    const auto textSize = ::Vector2AddValue(
                            gui::measure_text(text),
                            context_menu::style::padding);
                    _size.x = std::max(_size.x, textSize.x
                            + context_menu::style::padding * 2.0f);
                    _size.y += textSize.y;
                }
            }

            // inline end padding
            if (_opts.fitSize)
                _size.x += 50;
        }

    auto context_menu::render(void) const -> void {
        if (!_open)
            return;

        const auto elapsed = chrono::steady_clock::now() - _animationStart;
        const auto animationStep = std::clamp(
                chrono::duration<float>(elapsed)
                    / chrono::duration<float>(context_menu::style::animationDuration),
                0.0f, 1.0f);

        ::BeginScissorMode(_pos.x, _pos.y, _size.x, _size.y * animationStep);
        {
            ::DrawRectangleRec({
                        _pos.x,
                        _pos.y,
                        _size.x,
                        _size.y,
                    }, context_menu::style::backgroundColor);
            ::DrawRectangleLinesEx({
                        _pos.x,
                        _pos.y,
                        _size.x,
                        _size.y,
                    }, context_menu::style::borderThickness,
                    context_menu::style::borderColor);

            // draw items
            float yRelative = _pos.y;
            for (const auto& [text, _] : _items) {
                const auto textSize = gui::measure_text(text);
                const ::Rectangle rec {
                    _pos.x,
                    yRelative,
                    _size.x,
                    textSize.y + context_menu::style::padding * 2.0f,
                };

                // draw highlight
                if (::CheckCollisionPointRec(::GetMousePosition(), rec))
                    ::DrawRectangleRec(rec,
                            context_menu::style::highligthColor);

                gui::draw_text(text, {
                            _pos.x + context_menu::style::padding,
                            yRelative + context_menu::style::padding,
                        }, context_menu::style::textColor, false);
                yRelative += textSize.y + context_menu::style::padding * 2.0f;
            }
        }
        ::EndScissorMode();
    }

    auto context_menu::update(void) -> void {
        // TODO: implement context_menu::update()
    }
} // namespace widget
