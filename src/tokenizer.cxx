#include <format>

#include "tokenizer.hxx"

namespace fs = std::filesystem;

tokenizer::tokenizer_error::tokenizer_error(std::string_view message) noexcept {
    _message += message;
}

auto tokenizer::tokenizer_error::what(void) const noexcept -> const char* {
    return _message.c_str();
}

tokenizer::tokenizer(fs::path filePath) 
    : _filePath(std::move(filePath)) {}

auto tokenizer::tokens(void) const -> std::generator<tokenizer::token> {
    // TODO: not implemented yet
    throw tokenizer::tokenizer_error(
            "Function not implemented yet.");
}

auto tokenizer::token_type_name(tokenizer::token_t type)
    -> std::string_view {
        if (type == tokenizer::token_t::ERR)         return "ERR";
        else if (type == tokenizer::token_t::NUMBER) return "NUMBER";
        else if (type == tokenizer::token_t::STRING) return "STRING";
        else if (type == tokenizer::token_t::UUID)   return "UUID";
        else if (type == tokenizer::token_t::CSQARE) return "CSQARE";
        else if (type == tokenizer::token_t::DASH)   return "DASH";
        else if (type == tokenizer::token_t::OSQARE) return "OSQARE";

        throw tokenizer::tokenizer_error(
                std::format(
                    "Undefined token type name: {}.",
                    std::to_underlying(type)));
    }
