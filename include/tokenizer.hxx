#pragma once
#include <filesystem>
#include <generator>
#include <string>

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
            ERR,

            // value types
            NUMBER,
            STRING,
            UUID,

            // array syntax
            CSQARE,
            DASH,
            OSQARE,
        };

        struct token final {
            std::string text {};
            token_t type { token_t::ERR };
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

