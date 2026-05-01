#pragma once
#include <filesystem>
#include <generator>
#include <string>

#include "node.hxx"

class lexer final {
    public:
        class lexer_error final : public std::exception {
            public:
                lexer_error(std::string_view message = "Unknown error.") noexcept;

            public:
                auto what(void) const noexcept -> const char* override;

            private:
                std::string _message { "lexer error - " };
        };

    public:
        lexer(void) = delete;
        lexer(std::filesystem::path filePath);

        lexer(const lexer&) = delete;
        auto operator =(const lexer&) = delete;

        lexer(lexer&&) = delete;
        auto operator =(lexer&&) = delete;

        ~lexer(void) noexcept = default;

    public:
        auto nodes(void) const -> std::generator<node_ptr>;

    private:
        std::filesystem::path _filePath {};
};
