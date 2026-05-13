#pragma once
#include <filesystem>
#include <generator>
#include <string>

#include "node.hxx"

class parser final {
    public: // definitions
        class parser_error final : public std::exception {
            public: // ctors
                parser_error(std::string_view message = "Unknown error.") noexcept;

                parser_error(const parser_error&) noexcept = default;
                auto operator =(const parser_error&) noexcept -> parser_error& = default;

                parser_error(parser_error&&) noexcept = default;
                auto operator =(parser_error&&) noexcept -> parser_error& = default;

                ~parser_error(void) noexcept = default;

            public: // methods
                auto what(void) const noexcept -> const char* override;

            private: //members
                std::string _message { "-- Parser error - " };
        };

        struct string final {
            size_t row { 1 };
            size_t col { 1 };
            size_t indentLevel { 0 };
            std::string text {};
        };

    public: // ctors
        parser(void) = delete;
        parser(std::filesystem::path filePath);

        parser(const parser&) = delete;
        auto operator =(const parser&) = delete;

        parser(parser&&) = delete;
        auto operator =(parser&&) = delete;

        ~parser(void) noexcept = default;

    public: // methods
        auto strings(void) const -> std::generator<string>;

    private: // members
        std::filesystem::path _filePath {};
};
