#pragma once
#include <format>
#include <memory>
#include <string>
#include <vector>

#include "raylib.h"
#include "widget/base.hxx"

class node;
using node_ptr = std::shared_ptr<node>;
using const_node_ptr = std::shared_ptr<const node>;

class node final : public widget::base {
    public:
        class node_error final : public std::exception {
            public:
                node_error(std::string_view message = "Unknown error.") noexcept;

                node_error(const node_error&) noexcept = default;
                auto operator =(const node_error&) noexcept -> node_error& = default;

                node_error(node_error&&) noexcept = default;
                auto operator =(node_error&&) noexcept -> node_error& = default;

                ~node_error(void) noexcept = default;

            public:
                auto what(void) const noexcept -> const char* override;

            private:
                std::string _message { "-- Node error - " };
        };

    public:
        node(void) noexcept;
        node(std::string_view uuid, std::string_view title, std::string_view description, ::Vector2 position);

        node(const node&) = delete;
        auto operator =(const node&) = delete;

        node(node&&) = delete;
        auto operator =(node&&) = delete;

        ~node(void) noexcept = default;

    public:
        [[nodiscard]] auto uuid(void) const noexcept -> std::string;

        template<class Self>
            [[nodiscard]] auto&& title(this Self&& self) noexcept;
        template<class Self>
            [[nodiscard]] auto&& description(this Self&& self) noexcept;
        template<class Self>
            [[nodiscard]] auto&& position(this Self&& self) noexcept;
        template<class Self>
            [[nodiscard]] auto&& connections(this Self&& self) noexcept;

        auto title_size(void) const noexcept -> ::Vector2;
        auto description_size(void) const noexcept -> ::Vector2;
        auto size(void) const noexcept -> ::Vector2;

        auto update(void) -> void override;
        auto render(void) const -> void override;
        auto render_text(void) const -> void;

    public:
        static struct style {
            static inline constexpr float width   { 500 };
            static inline constexpr float padding { 10 };

            static inline constexpr float borderRoundness { 0.2f };
            static inline constexpr int   borderSegments  { 50 };
            static inline constexpr float borderThickness { 2 };

            static inline constexpr float connectionGap       { 20 };
            static inline constexpr float connectionThickness { 2 };

#ifdef DARK_THEME
            static inline constexpr ::Color backgroundColor { 20, 20, 20, 150 };
            static inline constexpr ::Color highligthColor  { 102, 155, 188, 150 };
            static inline constexpr ::Color borderColor     { ::RAYWHITE };
            static inline constexpr ::Color textColor       { ::RAYWHITE };
            static inline constexpr ::Color connectionColor { ::RAYWHITE };
#else
            static inline constexpr ::Color backgroundColor { 255, 255, 255, 150 };
            static inline constexpr ::Color highligthColor  { 162, 210, 255, 150 };
            static inline constexpr ::Color borderColor     { ::BLACK };
            static inline constexpr ::Color textColor       { ::BLACK };
            static inline constexpr ::Color connectionColor { ::BLACK };
#endif
        } style;

    private:
        struct node_text {
            std::string text {};
            std::string wrapped {};
        };

    private:
        std::string _uuid {};

        node_text _title {};
        ::Vector2 _titleSize {};

        node_text _description {};
        ::Vector2 _descriptionSize {};

        ::Vector2 _pos {};

        std::vector<std::string> _connections {};
};

template<class Self>
auto&& node::title(this Self&& self) noexcept {
    return std::forward_like<Self>(self._title);
}

template<class Self>
auto&& node::description(this Self&& self) noexcept {
    return std::forward_like<Self>(self._description);
}

template<class Self>
auto&& node::position(this Self&& self) noexcept {
    return std::forward_like<Self>(self._pos);
}

template<class Self>
auto&& node::connections(this Self&& self) noexcept {
    return std::forward_like<Self>(self._connections);
}
