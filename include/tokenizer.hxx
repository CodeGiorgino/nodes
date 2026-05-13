#pragma once
#include <filesystem>
#include <generator>
#include <string>
#include <vector>

#include "parser.hxx"

class tokenizer final {
    public: // definitions
        class tokenizer_error final : public std::exception {
            public: // ctors
                tokenizer_error(std::string_view message = "Unknown error.") noexcept;

                tokenizer_error(const tokenizer_error&) noexcept = default;
                auto operator =(const tokenizer_error&) noexcept -> tokenizer_error& = default;

                tokenizer_error(tokenizer_error&&) noexcept = default;
                auto operator =(tokenizer_error&&) noexcept -> tokenizer_error& = default;

                ~tokenizer_error(void) noexcept = default;

            public: // methods
                auto what(void) const noexcept -> const char* override;

            private: // members
                std::string _message { "-- Tokenizer error - " };
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

    public: // ctors
        tokenizer(void) = delete;
        tokenizer(std::filesystem::path filePath);

        tokenizer(const tokenizer&) = delete;
        auto operator =(const tokenizer&) = delete;

        tokenizer(tokenizer&&) = delete;
        auto operator =(tokenizer&&) = delete;

        ~tokenizer(void) = default;

    public: // methods
        auto tokens(void) const -> std::generator<token>;

        static auto token_type_name(token_t type) -> std::string_view;

    private: // members
        std::filesystem::path _filePath {};
};

