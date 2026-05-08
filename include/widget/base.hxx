#pragma once
#include <functional>
#include <memory>

#include "raylib.h"

namespace widget {
    class base;
    using base_ptr = std::shared_ptr<base>;

    class base {
        public:
            base(void) noexcept = default;
            base(::Rectangle rec) noexcept;

            base(const base&) noexcept = default;
            auto operator =(const base&) noexcept -> base& = default;

            base(base&&) noexcept = default;
            auto operator =(base&&) noexcept -> base& = default;

            virtual ~base(void) noexcept = 0;

        public:
            virtual auto check_collision(void) const noexcept -> bool;

            template<class Self>
                auto on_render(this Self&& self, std::function<void(const base&)> event);
            template<class Self>
                auto on_update(this Self&& self, std::function<void(base&)> event);

            virtual auto render(void) const -> void;
            virtual auto update(void) -> void;

        protected:
            ::Vector2 _pos {};
            ::Vector2 _size {};

            std::function<void(const base&)> _renderCallback {};
            std::function<void(base&)> _updateCallback {};
    };

    template<class Self>
        auto base::on_render(this Self&& self,
                std::function<void(const base&)> event) {
            self._renderCallback = event;
            return std::forward<Self>(self);
        }

    template<class Self>
        auto base::on_update(this Self&& self,
                std::function<void(base&)> event) {
            self._updateCallback = event;
            return std::forward<Self>(self);
        }
} // namespace widget
