#include <format>
#include <regex>

#include "uuid.hxx"
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
    parser p { _filePath };
    auto gen = p.words();
    auto it = gen.begin();

    tokenizer::token ret {};
    while (it != gen.end()) {
__tokenizer_scan_start:
        ret.words.clear();
        const auto& word = *it;

        // handle syntax tokens
        if (word.text == ",") {
            ret.type = tokenizer::token_t::COMMA;
            ret.words.push_back(word);
            co_yield ret;

            it++;
            continue;
        } else if (word.text == "]") {
            ret.type = tokenizer::token_t::CSQARE;
            ret.words.push_back(word);
            co_yield ret;

            it++;
            continue;
        } else if (word.text == "-") {
            ret.type = tokenizer::token_t::DASH;
            ret.words.push_back(word);
            co_yield ret;

            it++;
            continue;
        } else if (word.text == "[") {
            ret.type = tokenizer::token_t::OSQARE;
            ret.words.push_back(word);
            co_yield ret;

            it++;
            continue;
        } else if (word.text == ":") {
            ret.type = tokenizer::token_t::COLON;
            ret.words.push_back(word);
            co_yield ret;

            it++;
            continue;
        }

        // handle number tokens
        static const std::regex re(
                R"(^[-+]?(?:\d+(?:\.\d*)?|\.\d+)$)");

        if (std::regex_match(word.text, re)) {
            ret.type = tokenizer::token_t::NUMBER;
            ret.words.push_back(word);
            co_yield ret;

            it++;
            continue;
        }

        // handle multi-line string tokens
        if (word.text == ">" || word.text == "|") {
            if (word.text == ">")
                ret.type = tokenizer::token_t::STRING;
            else if (word.text == "|")
                ret.type = tokenizer::token_t::STRING_LITERAL;
            else throw tokenizer::tokenizer_error(
                    std::format(
                        "Multi-line string definition character not implemented ({:?}): [{}:{}:{}] [{}].",
                        word.text, _filePath.filename().string(),
                        word.row, word.col, _filePath.parent_path().string()));

            if (++it == gen.end()) {
                co_yield ret;
                continue;
            }

            const auto indentLevel = (*it).indentLevel;
            while(it != gen.end()) {
                const auto& nextWord = *it;
                if (nextWord.indentLevel != indentLevel) {
                    co_yield ret;
                    goto __tokenizer_scan_start;
                } else ret.words.push_back(nextWord);
                
                it++;
            };

            co_yield ret;

            it++;
            continue;
        }

        // handle UUID tokens
        if (uuid::test_v4(word.text)) {
            ret.type = tokenizer::token_t::UUID;
            ret.words.push_back(word);
            co_yield ret;

            it++;
            continue;
        }

        // defaults to string token
        ret.type = tokenizer::token_t::STRING;
        ret.words.push_back(word);
        co_yield ret;

        it++;
    };
}

auto tokenizer::token_type_name(tokenizer::token_t type)
    -> std::string_view {
        switch (type) {
            case tokenizer::token_t::NUMBER:
                return "NUMBER";
            case tokenizer::token_t::STRING:
                return "STRING";
            case tokenizer::token_t::STRING_LITERAL:
                return "STRING_LITERAL";
            case tokenizer::token_t::UUID:
                return "UUID";
            case tokenizer::token_t::COMMA:
                return "COMMA";
            case tokenizer::token_t::CSQARE:
                return "CSQARE";
            case tokenizer::token_t::DASH:
                return "DASH";
            case tokenizer::token_t::OSQARE:
                return "OSQARE";
            case tokenizer::token_t::COLON:
                return "COLON";
        };

        throw tokenizer::tokenizer_error(
                std::format(
                    "Undefined token type name: {}.",
                    std::to_underlying(type)));
    }
