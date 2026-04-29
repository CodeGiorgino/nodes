#include <format>

#include "tokenizer.hxx"

auto tokenizer::tokens(void) const -> std::generator<tokenizer::token> {
    
}

constexpr auto tokenizer::token_type_name(tokenizer::token_t type)
    -> std::string_view {
        if (type == tokenizer::token_t::ERR)         return "ERR";
        else if (type == tokenizer::token_t::NUMBER) return "NUMBER";
        else if (type == tokenizer::token_t::STRING) return "STRING";
        else if (type == tokenizer::token_t::UUID)   return "UUID";
        else if (type == tokenizer::token_t::CSQARE) return "CSQARE";
        else if (type == tokenizer::token_t::DASH)   return "DASH";
        else if (type == tokenizer::token_t::OSQARE) return "OSQARE";
        else if (type == tokenizer::token_t::GT)     return "GT";
        else if (type == tokenizer::token_t::PIPE)   return "PIPE";

        throw std::runtime_error(
                std::format(
                    "token error - undefined token type name: {}",
                    std::to_underlying(type)));
    }
