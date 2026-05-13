#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "node.hxx"
#include "widget/base.hxx"
#include "widget/context_menu.hxx"

class scene final {
    public: // definitions
        class scene_error final : public std::exception {
            public: // ctors
                scene_error(std::string_view message = "Unknown error.") noexcept;

                scene_error(const scene_error&) noexcept = default;
                auto operator =(const scene_error&) noexcept -> scene_error& = default;

                scene_error(scene_error&&) noexcept = default;
                auto operator =(scene_error&&) noexcept -> scene_error& = default;

                ~scene_error(void) noexcept = default;

            public: // methods
                auto what(void) const noexcept -> const char* override;

            private: // members
                std::string _message { "-- Scene error - " };
        };

        static struct style {
#ifdef DARK_THEME
            static inline constexpr ::Color backgroundColor { ::BLACK };
            static inline constexpr ::Color gridColor       { ::LIGHTGRAY };
#else
            static inline constexpr ::Color backgroundColor { ::RAYWHITE };
            static inline constexpr ::Color gridColor       { ::LIGHTGRAY };
#endif
        } style;

    public: // ctors
        scene(void) noexcept = default;
        scene(std::filesystem::path configFilePath);

        scene(const scene&) noexcept = delete;
        auto operator =(const scene&) noexcept -> scene& = delete;

        scene(scene&&) noexcept = delete;
        auto operator =(scene&&) noexcept -> scene& = delete;

        ~scene(void) noexcept = default;

    public: // methods
        auto load(void) -> void;
        auto load(std::filesystem::path configFilePath) -> void;

        auto render(void) const -> void;
        auto update(void) -> void;

    private: // methods
        auto render_grid(void) const -> void;
        auto render_nodes(void) const -> void;
        auto render_widgets(void) const -> void;

    private: // members
        std::filesystem::path _configFilePath {};

        std::vector<node::shared_ptr> _nodes {};
        std::unordered_map<std::string, std::vector<node::shared_ptr>> _nodeMap {};

        node::shared_ptr _focusedNode { nullptr };
        widget::context_menu::shared_ptr _menu { nullptr };
};
