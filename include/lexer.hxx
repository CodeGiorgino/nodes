#pragma once
#include <filesystem>
#include <generator>
#include <string>

#include "node.hxx"

class lexer final {
    public: // definitions
        class lexer_error final : public std::exception {
            public: // ctors
                lexer_error(std::string_view message = "Unknown error.") noexcept;

                lexer_error(const lexer_error&) noexcept = default;
                auto operator =(const lexer_error&) noexcept -> lexer_error& = default;

                lexer_error(lexer_error&&) noexcept = default;
                auto operator =(lexer_error&&) noexcept -> lexer_error& = default;

                ~lexer_error(void) = default;

            public: // methods
                auto what(void) const noexcept -> const char* override;

            private: // members
                std::string _message { "-- Lexer error - " };
        };

    public: // ctors
        lexer(void) = delete;
        lexer(std::filesystem::path filePath);

        lexer(const lexer&) = delete;
        auto operator =(const lexer&) = delete;

        lexer(lexer&&) = delete;
        auto operator =(lexer&&) = delete;

        ~lexer(void) noexcept = default;

    public: // methods
        auto nodes(void) const -> std::generator<node::shared_ptr>;

    private: // members
        std::filesystem::path _filePath {};
};
