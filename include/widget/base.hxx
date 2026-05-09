#pragma once
#include <functional>
#include <memory>

#include "raylib.h"

namespace widget {
    template<class Derived = void>
        class base : std::enable_shared_from_this<Derived> {
            public:
                using base_ptr = std::shared_ptr<base>;
                using const_base_ptr = std::shared_ptr<const base>;

                using shared_ptr = std::shared_ptr<Derived>;
                using const_shared_ptr = std::shared_ptr<const Derived>;

            public:
                base(void) noexcept = default;

                base(::Rectangle rec) noexcept {
                    _pos = { rec.x, rec.y };
                    _size = { rec.width, rec.height };
                }

                base(const base&) noexcept = default;
                auto operator =(const base&) noexcept -> base& = default;

                base(base&&) noexcept = default;
                auto operator =(base&&) noexcept -> base& = default;

                virtual ~base(void) noexcept {}

            public:
                virtual auto check_collision(void) const noexcept -> bool {
                    return ::CheckCollisionPointRec(::GetMousePosition(), {
                                _pos.x,
                                _pos.y,
                                _size.x,
                                _size.y,
                            });
                }

                template<class Self>
                    auto&& on_render(this Self&& self, std::function<void(const const_shared_ptr)> event) {
                        self._renderCallback = event;
                        return std::forward<Self>(self);
                    }

                template<class Self>
                    auto&& on_update(this Self&& self, std::function<void(const shared_ptr)> event) {
                        self._updateCallback = event;
                        return std::forward<Self>(self);
                    }

                virtual auto render(void) const -> void {
                    if (_renderCallback)
                        _renderCallback(this->shared_from_this());
                }

                virtual auto update(void) -> void {
                    if (_updateCallback)
                        _updateCallback(this->shared_from_this());
                }

            protected:
                ::Vector2 _pos {};
                ::Vector2 _size {};

                std::function<void(const const_shared_ptr)> _renderCallback {};
                std::function<void(const shared_ptr)> _updateCallback {};
        };
} // namespace widget
