#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "node.hxx"

class scene final {
    public:
        class scene_error final : public std::exception {
            public:
                scene_error(std::string_view message = "Unknown error.") noexcept;

                scene_error(const scene_error&) noexcept = default;
                auto operator =(const scene_error&) noexcept -> scene_error& = default;

                scene_error(scene_error&&) noexcept = default;
                auto operator =(scene_error&&) noexcept -> scene_error& = default;

                ~scene_error(void) noexcept = default;

            public:
                auto what(void) const noexcept -> const char* override;

            private:
                std::string _message { "-- Scene error - " };
        };

    public:
        scene(void) noexcept = default;
        scene(std::filesystem::path configFilePath);

        scene(const scene&) noexcept = delete;
        auto operator =(const scene&) noexcept -> scene& = delete;

        scene(scene&&) noexcept = delete;
        auto operator =(scene&&) noexcept -> scene& = delete;

        ~scene(void) noexcept = default;

    public:
        static struct style {
#ifdef DARK_THEME
            static inline constexpr ::Color backgroundColor { ::BLACK };
            static inline constexpr ::Color gridColor { ::LIGHTGRAY };
#else
            static inline constexpr ::Color backgroundColor { ::RAYWHITE };
            static inline constexpr ::Color gridColor { ::LIGHTGRAY };
#endif
        } style;

    public:
        auto load(void) -> void;
        auto load(std::filesystem::path configFilePath) -> void;

        auto render(void) const -> void;
        auto update(void) -> void;

    private:
        auto render_grid(void) const noexcept -> void;
        auto render_nodes(void) const -> void;

    private:
        std::filesystem::path _configFilePath {};

        std::vector<node_ptr> _nodes {};
        std::unordered_map<std::string, std::vector<node_ptr>> _nodeMap {};
};
