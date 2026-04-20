#pragma once

#include "raylib.h"

class enviroment final {
    public:
        enviroment(const enviroment&) = delete;
        enviroment(enviroment&&) = delete;

    public:
        static auto get_instance(void) noexcept -> enviroment&;

        template<class Self>
        auto& get_camera(this Self&& self) noexcept {
            return self.camera;
        }

    private:
        enviroment() = default;

    public:
        static constexpr int gridSize { 100 };

    private:
        ::Camera2D camera {};
};
