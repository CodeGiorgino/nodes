#pragma once
#include <memory>
#include <string>
#include <unordered_map>

class node final {
    public:
        node(void) noexcept;
        node(std::string_view uuid, std::string_view title, std::string_view description, ::Vector2 pos);
        ~node(void) noexcept = default;

        template<class Self>
        [[nodiscard]] auto& title(this Self&& self) noexcept;

    public:
        auto update(void) noexcept -> void;
        auto render(void) const noexcept -> void;

    private:
        static constexpr float width { 400 };
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

        // TODO: list of tasks
};

using node_ptr = std::shared_ptr<node>;
using node_map = std::unordered_map<std::string, node_ptr>;

template<class Self>
auto& node::title(this Self&& self) noexcept {
    return std::forward_like<Self>(self.title);
}
