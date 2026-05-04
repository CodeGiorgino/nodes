#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>
#include <utility>

#include "raylib.h"

class enviroment final {
    public:
        class enviroment_error final : public std::exception {
            public:
                enviroment_error(std::string_view message = "Unknown error.") noexcept;

            public:
                auto what(void) const noexcept -> const char* override;

            private:
                std::string _message { "enviroment error - " };
        };

    public:
        enviroment(const enviroment&) = delete;
        auto operator =(const enviroment&) = delete;

        enviroment(enviroment&&) = delete;
        auto operator =(enviroment&&) = delete;

        ~enviroment(void) noexcept = default;

    public:
        static auto get_instance(void) noexcept -> enviroment&;

        auto init(std::string_view program, std::filesystem::path configFilePath) -> void;
        auto deinit(void) noexcept -> void;

        template<class Self>
        [[nodiscard]] auto& camera(this Self&& self) noexcept;
        auto load_font(unsigned fontSize = fontSizeDefault) -> void;
        [[nodiscard]] auto font(void) const -> std::pair<::Font, unsigned>;
        [[nodiscard]] auto font_default(void) const -> std::pair<::Font, unsigned>;

    private:
        enviroment(void) = default;

    public:
        static constexpr int gridSize { 50 };
        static constexpr int fontSizeDefault { 20 };

    private:
        std::filesystem::path _program {};
        std::filesystem::path _configFilePath {};

        ::Camera2D _camera {};

        unsigned _fontSize { fontSizeDefault };
        std::unordered_map<unsigned, ::Font> _fontMap {};
        ::Font _fontDefault {};
};

template<class Self>
auto& enviroment::camera(this Self&& self) noexcept {
    return std::forward_like<Self>(self._camera);
}
