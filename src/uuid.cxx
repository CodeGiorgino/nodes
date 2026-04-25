#include <regex>
#include <sstream>

#include "uuid.hxx"

namespace uuid {
    // Source - https://stackoverflow.com/a/60198074
    auto generate_v4(void) noexcept -> std::string {
        std::stringstream ss;
        ss << std::hex;

        for (int i = 0; i < 8; i++)
            ss << distribution(generator);
        ss << "-";

        for (int i = 0; i < 4; i++)
            ss << distribution(generator);
        ss << "-4";

        for (int i = 0; i < 3; i++)
            ss << distribution(generator);
        ss << "-";

        ss << distribution2(generator);

        for (int i = 0; i < 3; i++)
            ss << distribution(generator);
        ss << "-";

        for (int i = 0; i < 12; i++)
            ss << distribution(generator);

        return ss.str();
    }

    auto test_v4(std::string_view text) noexcept -> bool {
        if (text.empty())
            return false;

        // Source - https://stackoverflow.com/a/38191104
        static const std::regex re (
            "^[0-9A-F]{8}"
            "-[0-9A-F]{4}"
            "-[4][0-9A-F]{3}"
            "-[89AB][0-9A-F]{3}"
            "-[0-9A-F]{12}$",
            std::regex::icase);

        const std::string textStr { text };
        return std::regex_match(textStr, re);
    }
} // namespace uuid
