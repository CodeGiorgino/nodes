#pragma once
#include <chrono>
#include <functional>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

#include "widget/base.hxx"

namespace widget {
    class context_menu final : public base<context_menu> {
        public:
            struct options {
                bool fitSize { false };
            };

            using on_click = std::function<void(void)>;
            using item = std::pair<std::string, on_click>;

        public:
            static auto create(::Rectangle rec, std::initializer_list<context_menu::item> items, context_menu::options opts) noexcept -> shared_ptr;

            context_menu(const context_menu&) noexcept = default;
            auto operator =(const context_menu&) noexcept -> context_menu& = default;

            context_menu(context_menu&&) noexcept = default;
            auto operator =(context_menu&&) noexcept -> context_menu& = default;

            ~context_menu(void) noexcept = default;

        public:
            auto check_collision(void) const noexcept -> bool override;

            template<class Self>
                auto&& open(this Self&& self) noexcept;
            template<class Self>
                auto&& animation_start(this Self&& self) noexcept;

            auto render(void) const -> void override;
            auto update(void) -> void override;

        public:
            static struct style {
                static inline constexpr float padding { 10 };

                static inline constexpr float borderThickness { 1 };

                static inline constexpr std::chrono::milliseconds animationDuration { 150 };

#ifdef DARK_THEME
                static inline constexpr ::Color backgroundColor { ::BLACK };
                static inline constexpr ::Color borderColor     { ::RAYWHITE };
                static inline constexpr ::Color highligthColor  { 255, 255, 255, 25 };
                static inline constexpr ::Color textColor       { ::RAYWHITE };
#else
                static inline constexpr ::Color backgroundColor { ::RAYWHITE };
                static inline constexpr ::Color borderColor     { ::BLACK };
                static inline constexpr ::Color highligthColor  { 0, 0, 0, 25 };
                static inline constexpr ::Color textColor       { ::BLACK };
#endif
            } style;

        private:
            context_menu(void) = delete;
            context_menu(::Rectangle rec, std::initializer_list<context_menu::item> items, context_menu::options opts) noexcept;

        private:
            context_menu::options _opts;
            std::vector<context_menu::item> _items {};

            bool _open { false };
            std::chrono::steady_clock::time_point _animationStart {};
    };

    template<class Self>
        auto&& context_menu::open(this Self&& self) noexcept {
            return std::forward_like<Self>(self._open);
        }

    template<class Self>
        auto&& context_menu::animation_start(this Self&& self) noexcept {
            return std::forward_like<Self>(self._animationStart);
        }
} // namespace widget
