#pragma once
#include <generator>
#include <string>

class tokenizer final {
    public:
        enum class token_t {
            ERR,

            // value types
            NUMBER,
            STRING,
            UUID,

            // array syntax
            CSQARE,
            DASH,
            OSQARE,

            // string syntax
            GT,
            PIPE,
        };

        struct token final {
            std::string text {};
            token_t type { token_t::ERR };
        };

    public:
        tokenizer(std::string_view filePath);
        tokenizer(const tokenizer&) = delete;
        tokenizer(tokenizer&&) = delete;
        ~tokenizer(void) noexcept = default;

    public:
        auto tokens(void) const -> std::generator<token>;
        static constexpr auto token_type_name(token_t type) -> std::string_view;

    private:
        std::string_view _filePath {};
};

