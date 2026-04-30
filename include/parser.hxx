#pragma once
#include <filesystem>
#include <generator>
#include <string>

#include "node.hxx"

class parser final {
    public:
        class parser_error final : public std::exception {
            public:
                parser_error(std::string_view message = "Unknown error.") noexcept;

            public:
                auto what(void) const noexcept -> const char* override;

            private:
                std::string _message { "parser error - " };
        };

        struct string final {
            size_t row { 0 };
            size_t col { 0 };
            size_t indentLevel { 0 };
            std::string text {};
        };

    public:
        parser(std::filesystem::path filePath);
        parser(const parser&)  = delete;
        parser(parser&&)       = delete;
        ~parser(void) noexcept = default;

    public:
        auto strings(void) const -> std::generator<string>;

    private:
        std::filesystem::path _filePath {};
};
