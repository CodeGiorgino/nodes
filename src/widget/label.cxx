#include "gui.hxx"
#include "widget/label.hxx"

namespace widget {
    label::label(::Rectangle rec, std::string_view text, label::options opts)
        noexcept :
        widget::base<label>(std::forward<::Rectangle>(rec)),
        _opts(std::move(opts)) {
            _text = text;
            if (opts.fitSize) {
                _size = gui::measure_text(text);
            } else if (opts.textWrap) {
                _text = gui::wrap_text(text, _size.x);
                _size = gui::measure_text(_text);
            }
        }

    auto label::create(::Rectangle rec, std::string_view text,
            label::options opts) noexcept -> shared_ptr {
        return std::make_shared<label>(label {
                    std::forward<::Rectangle>(rec),
                    std::forward<std::string_view>(text),
                    std::forward<label::options>(opts),
                });
    }

    auto label::render(void) const -> void {
        gui::draw_text(_text, _pos, label::style::textColor, _opts.scaled);
    }
} // namespace widget
