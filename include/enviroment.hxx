#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>
#include <utility>

#include "raylib.h"

class enviroment final {
    public: // definitions
        class enviroment_error final : public std::exception {
            public: // ctors
                enviroment_error(std::string_view message = "Unknown error.") noexcept;

                enviroment_error(const enviroment_error&) noexcept = default;
                auto operator =(const enviroment_error&) noexcept -> enviroment_error& = default;

                enviroment_error(enviroment_error&&) noexcept = default;
                auto operator =(enviroment_error&&) noexcept -> enviroment_error& = default;

                ~enviroment_error(void) noexcept = default;

            public: // methods
                auto what(void) const noexcept -> const char* override;

            private: // members
                std::string _message { "-- Enviroment error - " };
        };

        static inline constexpr int gridSize { 50 };
        static inline constexpr int fontSizeDefault { 20 };

    public: // ctors
        enviroment(const enviroment&) = delete;
        auto operator =(const enviroment&) = delete;

        enviroment(enviroment&&) = delete;
        auto operator =(enviroment&&) = delete;

        ~enviroment(void) noexcept = default;

    public: // methods
        static auto get_instance(void) noexcept -> enviroment&;

        auto init(std::string_view program, std::filesystem::path configFilePath) -> void;
        auto deinit(void) noexcept -> void;

        template<class Self>
            [[nodiscard]] auto&& camera(this Self&& self) noexcept;

        auto load_font(unsigned fontSize = fontSizeDefault) -> void;
        [[nodiscard]] auto font(void) const -> std::pair<::Font, unsigned>;
        [[nodiscard]] auto font_default(void) const -> std::pair<::Font, unsigned>;

    private: // ctors
        enviroment(void) = default;

    private: // members
        std::filesystem::path _program {};
        std::filesystem::path _configFilePath {};

        ::Camera2D _camera {};

        unsigned _fontSize { fontSizeDefault };
        std::unordered_map<unsigned, ::Font> _fontMap {};
        ::Font _fontDefault {};
};

template<class Self>
auto&& enviroment::camera(this Self&& self) noexcept {
    return std::forward_like<Self>(self._camera);
}
