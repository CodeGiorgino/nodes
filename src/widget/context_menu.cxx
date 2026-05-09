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
            if (_opts.fitSize)
                _size = {};

            for (const auto& [text, e] : items) {
                _items.emplace_back(text, e);
                if (_opts.fitSize) {
                    const auto textSize = ::Vector2AddValue(
                            gui::measure_text(text),
                            context_menu::style::padding);
                    _size.x = std::max(_size.x, textSize.x);
                    _size.y += textSize.y;
                }
            }
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
            ::DrawRectangleRounded({
                        _pos.x,
                        _pos.y,
                        _size.x,
                        _size.y,
                    }, context_menu::style::borderRoundness,
                    context_menu::style::borderSegments,
                    context_menu::style::backgroundColor);
        }
        ::EndScissorMode();
    }

    auto context_menu::update(void) -> void {
        // TODO: implement context_menu::update()
    }
} // namespace widget
