#pragma once
#include <random>
#include <string>

namespace uuid {
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::uniform_int_distribution<> distribution(0, 15);
    static std::uniform_int_distribution<> distribution2(8, 11);

    [[nodiscard]] auto generate_v4(void) noexcept -> std::string;
    [[nodiscard]] auto test_v4(std::string_view text) noexcept -> bool;
}
