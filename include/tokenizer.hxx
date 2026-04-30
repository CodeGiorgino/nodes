#pragma once
#include <filesystem>
#include <generator>
#include <string>
#include <vector>

#include "parser.hxx"

class tokenizer final {
    public:
        class tokenizer_error final : public std::exception {
            public:
                tokenizer_error(std::string_view message = "Unknown error.") noexcept;

            public:
                auto what(void) const noexcept -> const char* override;

            private:
                std::string _message { "tokenizer error - " };
        };

        enum class token_t {
            // value types
            NUMBER,
            STRING,
            STRING_LITERAL,
            UUID,

            // array syntax
            COMMA,
            CSQARE,
            DASH,
            OSQARE,

            // other
            COLON,
        };

        struct token final {
            token_t type { token_t::STRING };
            std::vector<parser::string> strings {};

            auto text(void) const noexcept -> std::string;
        };

    public:
        tokenizer(std::filesystem::path filePath);
        tokenizer(const tokenizer&) = delete;
        tokenizer(tokenizer&&) = delete;
        ~tokenizer(void) noexcept = default;

    public:
        auto tokens(void) const -> std::generator<token>;
        static auto token_type_name(token_t type) -> std::string_view;

    private:
        std::filesystem::path _filePath {};
};

