#include <ranges>

#include "enviroment.hxx"
#include "gui.hxx"

namespace views = std::views;

namespace gui {
    auto measure_text(std::string_view text) noexcept -> ::Vector2 {
        if (text.empty())
            return {};

        const auto& env = enviroment::get_instance();
        const auto& font = env.font();

        const std::string textStr { text };
        return ::MeasureTextEx(font, textStr.c_str(), env.fontSize, 1);
    }

    auto wrap_text(std::string_view text, float maxWidth) noexcept
        -> std::string {
            if (text.empty())
                return {};

            std::string ret {};
            std::string line {};
            for (const auto part : views::split(text, ' ')) {
                const std::string word { std::string_view { part } };
                const std::string testLine =
                    line.empty() ? word : (line + ' ' + word);

                const auto textSize = gui::measure_text(testLine);

                if (textSize.x > maxWidth) {
                    ret += ret.empty() ? line : ('\n' + line);
                    line = word;
                } else line = std::move(testLine);
            }

            ret += ret.empty() ? line : ('\n' + line);
            return ret;
        }

    auto draw_text(std::string_view text, ::Vector2 pos, ::Color tint) noexcept
        -> void {
            if (text.empty())
                return;

            const auto& env = enviroment::get_instance();
            const auto& font = env.font();

            const std::string textStr { text };
            ::DrawTextEx(font, textStr.c_str(), pos, env.fontSize, 1, tint);
        }

    auto draw_text_rec(std::string_view text, ::Vector2 recPos, float width,
            float padding, ::Color color, ::Color tint) noexcept -> ::Vector2 {
        if (text.empty())
            return {};

        const std::string textStr { text };
        const auto textSize = gui::measure_text(text);

        const ::Vector2 recSize {
            width,
            textSize.y + padding * 2,
        };

        const ::Vector2 textPos {
            recPos.x + padding,
            recPos.y + padding,
        };

        ::DrawRectangleV(recPos, recSize, color);
        ::DrawRectangleLines(recPos.x, recPos.y, recSize.x, recSize.y, tint);

        gui::draw_text(text, textPos);

        return recSize;
    }
} // namespace gui
