#include "gui.hxx"
#include "widget/label.hxx"

namespace widget {
    label::label(::Rectangle rec, std::string_view text, label::options opts)
        noexcept :
        base(std::forward<::Rectangle>(rec)),
        _opts(std::move(opts)) {
            _text = text;
            if (opts.fitSize) {
                _size = gui::measure_text(text);
            } else if (opts.textWrap) {
                _text = gui::wrap_text(text, _size.x);
                _size = gui::measure_text(_text);
            }
        }

    label::~label(void) noexcept {}

    auto label::render(void) const -> void {
        base::render();
        gui::draw_text(_text, _pos, label::style::textColor, _opts.textScaled);
    }

    auto label::update(void) -> void {
        base::update();
    }
} // namespace widget
