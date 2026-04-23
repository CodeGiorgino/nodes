#pragma once
#include <string>

#include "enviroment.hxx"
#include "raylib.h"

namespace gui {
    [[nodiscard]]
    auto measure_text(std::string_view text)
        noexcept -> ::Vector2;
    [[nodiscard]]
    auto wrap_text(std::string_view text, float maxWidth)
        noexcept -> std::string;

    auto draw_text(std::string_view text, ::Vector2 pos, ::Color tint = ::BLACK)
        noexcept -> void;
    auto draw_text_rec(std::string_view text, ::Vector2 recPos, float width,
            float padding = 10.0f, ::Color color = ::ColorAlpha(::LIGHTGRAY, 0.2f),
            ::Color tint = ::BLACK)
        noexcept -> ::Vector2;
} // namespace gui
