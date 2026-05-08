#include "widget/base.hxx"

namespace widget {
    base::base(::Rectangle rec) noexcept {
        _pos = { rec.x, rec.y };
        _size = { rec.width, rec.height };
    }

    base::~base(void) noexcept {}

    auto base::check_collision(void) const noexcept -> bool {
        return ::CheckCollisionPointRec(::GetMousePosition(), {
                    _pos.x,
                    _pos.y,
                    _size.x,
                    _size.y,
                });
    }

    auto base::render(void) const -> void {
        if (_renderCallback)
            _renderCallback(*this);
    }

    auto base::update(void) -> void {
        if (_updateCallback)
            _updateCallback(*this);
    }
} // namespace widget
