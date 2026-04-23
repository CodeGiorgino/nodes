#pragma once
#include <filesystem>
#include <string>
#include <utility>

#include "raylib.h"

class enviroment final {
    public:
        enviroment(const enviroment&)
           noexcept = delete;
        enviroment(enviroment&&)
           noexcept = delete;
        ~enviroment(void)
           noexcept = default;

    public:
        static auto get_instance(void)
            noexcept -> enviroment&;

        auto init(std::string_view program)
            -> void;
        auto deinit(void)
            noexcept -> void;

        template<class Self> [[nodiscard]]
        auto& camera(this Self&& self) noexcept {
            return std::forward_like<Self>(self._camera);
        }

        template<class Self> [[nodiscard]]
        auto font(this Self&& self) noexcept {
            return std::forward_like<Self>(self._font);
        }

    private:
        enviroment(void) = default;

    public:
        static constexpr int gridSize { 100 };
        static constexpr int fontSize { 20 };

    private:
        std::filesystem::path _program {};
        ::Camera2D _camera {};
        ::Font _font {};
};
