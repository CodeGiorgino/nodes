// https://stackoverflow.com/a/60198074

#pragma once
#include <random>
#include <sstream>
#include <string>

namespace uuid {
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::uniform_int_distribution<> distribution(0, 15);
    static std::uniform_int_distribution<> distribution2(8, 11);

    auto generate_v4(void) noexcept -> std::string;
}
