#pragma once
#include <format>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "raylib.h"

class node;
using node_ptr = std::shared_ptr<node>;
using node_map = std::unordered_map<std::string, std::vector<node_ptr>>;

class node final {
    public:
        class node_error final : public std::exception {
            public:
                node_error(std::string_view message = "Unknown error.") noexcept;

            public:
                auto what(void) const noexcept -> const char* override;

            private:
                std::string _message { "node error - " };
        };

        enum class connection_dir {
            TO,
            FROM,
        };

        struct connection {
            std::string uuid {};
            connection_dir dir { connection_dir::TO };
        };

    public:
        node(void) noexcept;
        node(std::string_view uuid, std::string_view title, std::string_view description, ::Vector2 pos);
        node(const node&)    = delete;
        node(node&&)         = delete;
        ~node(void) noexcept = default;

    public:
        [[nodiscard]] auto uuid(void) const noexcept -> std::string;
        template<class Self>
        [[nodiscard]] auto& title(this Self&& self) noexcept;
        template<class Self>
        [[nodiscard]] auto& description(this Self&& self) noexcept;

        auto update(void) noexcept -> void;
        auto render(void) const noexcept -> void;
        auto render_text(void) const noexcept -> void;

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
        node_text _description {};
        ::Vector2 _pos {};
};

template<class Self>
auto& node::title(this Self&& self) noexcept {
    return std::forward_like<Self>(self.title);
}

template<class Self>
auto& node::description(this Self&& self) noexcept {
    return std::forward_like<Self>(self._description);
}
