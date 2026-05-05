#pragma once
#include <format>
#include <memory>
#include <string>
#include <vector>

#include "raylib.h"

class node;
using node_ptr = std::shared_ptr<node>;

class node final {
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
        node(std::string_view uuid, std::string_view title, std::string_view description, ::Vector2 pos);

        node(const node&) = delete;
        auto operator =(const node&) = delete;

        node(node&&) = delete;
        auto operator =(node&&) = delete;

        ~node(void) noexcept = default;

    public:
        [[nodiscard]] auto uuid(void) const noexcept -> std::string;

        template<class Self>
        [[nodiscard]] auto& title(this Self&& self) noexcept;

        template<class Self>
        [[nodiscard]] auto& description(this Self&& self) noexcept;

        auto title_size(void) const noexcept -> ::Vector2;
        auto description_size(void) const noexcept -> ::Vector2;
        auto size(void) const noexcept -> ::Vector2;

        template<class Self>
        [[nodiscard]] auto& position(this Self&& self) noexcept;

        template<class Self>
        [[nodiscard]] auto& connections(this Self&& self) noexcept;

        auto update(void) noexcept -> void;
        auto render(void) const noexcept -> void;
        auto render_text(void) const noexcept -> void;

    public:
        static constexpr float connectionGap { 20 };

    private:
        static constexpr float width { 500 };
        static constexpr float padding { 10 };

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
auto& node::title(this Self&& self) noexcept {
    return std::forward_like<Self>(self._title);
}

template<class Self>
auto& node::description(this Self&& self) noexcept {
    return std::forward_like<Self>(self._description);
}

template<class Self>
auto& node::position(this Self&& self) noexcept {
    return std::forward_like<Self>(self._pos);
}

template<class Self>
auto& node::connections(this Self&& self) noexcept {
    return std::forward_like<Self>(self._connections);
}
