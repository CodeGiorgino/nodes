#pragma once
#include <string>

#include "widget/base.hxx"

namespace widget {
    class label final : public base<label> {
        public: // definitions
            struct options {
                bool fitSize  { false };
                bool scaled   { false };
                bool textWrap { false };
            };

            static struct style {
#ifdef DARK_THEME
                static inline constexpr ::Color textColor { ::RAYWHITE };
#else
                static inline constexpr ::Color textColor { ::BLACK };
#endif
            } style;

        public: // ctors
            label(const label&) noexcept = default;
            auto operator =(const label&) noexcept -> label& = default;

            label(label&&) noexcept = default;
            auto operator =(label&&) noexcept -> label& = default;

            ~label(void) noexcept = default;

        public: // methods
            static auto create(::Rectangle rec, std::string_view text, label::options opts) noexcept -> shared_ptr;

            auto render(void) const -> void override;
            auto update(void) -> void override {}

        private: // ctors
            label(void) = delete;
            label(::Rectangle rec, std::string_view text, label::options opts) noexcept;

        private: // members
            std::string _text {};
            label::options _opts {};
    };
} // namespace widget
