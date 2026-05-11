#pragma once
#include <functional>
#include <memory>

#include "raylib.h"

namespace widget {
    template<class Derived = void>
        class base : public std::enable_shared_from_this<Derived> {
            public:
                using base_ptr = std::shared_ptr<base>;
                using const_base_ptr = std::shared_ptr<const base>;

                using shared_ptr = std::shared_ptr<Derived>;
                using const_shared_ptr = std::shared_ptr<const Derived>;

            public:
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
                static auto create(void) noexcept -> shared_ptr {
                    return std::make_shared<Derived>();
                }

                auto position(void) const noexcept -> ::Vector2 {
                    return _pos;
                }

                auto size(void) const noexcept -> ::Vector2 {
                    return _size;
                }

                virtual auto check_collision(void) const noexcept -> bool {
                    return ::CheckCollisionPointRec(::GetMousePosition(), {
                                _pos.x,
                                _pos.y,
                                _size.x,
                                _size.y,
                            });
                }

                virtual auto render(void) const -> void = 0;
                virtual auto update(void) -> void = 0;

            protected:
                base(void) = default;

            protected:
                ::Vector2 _pos {};
                ::Vector2 _size {};

                std::function<void(const const_shared_ptr)> _renderCallback {};
                std::function<void(const shared_ptr)> _updateCallback {};
        };
} // namespace widget
