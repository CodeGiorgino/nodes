#include "uuid.hxx"

namespace uuid {
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
} // namespace uuid
