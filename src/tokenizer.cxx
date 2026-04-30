#include <algorithm>
#include <format>
#include <ranges>
#include <regex>

#include "uuid.hxx"
#include "tokenizer.hxx"

namespace fs = std::filesystem;
namespace ranges = std::ranges;

tokenizer::tokenizer_error::tokenizer_error(std::string_view message) noexcept {
    _message += message;
}

auto tokenizer::tokenizer_error::what(void) const noexcept -> const char* {
    return _message.c_str();
}

auto tokenizer::token::text(void) const noexcept -> std::string {
    std::string ret {};
    for (size_t i = 0; i < strings.size(); i++) {
        const auto& str = strings[i];
        if (str.text.empty())
            continue;

        if (type == tokenizer::token_t::STRING_LITERAL) {
            if (!ret.empty()) {
                if (i > 0) {
                    const auto& lastWord = strings[i - 1];
                    if (lastWord.row < str.row)
                        ret += '\n';
                }
            }

            ret += str.text;
            if (!std::isalnum(str.text[str.text.size() - 1]))
                ret += ' ';
        } else {
            ret += str.text;
            if (!std::isalnum(str.text[str.text.size() - 1]))
                ret += ' ';
        }
    }

    if (ret.empty())
        return {};

    size_t start = 0;
    size_t end = ret.size() - 1;
    for (; start < ret.size()
            && std::isspace(ret[start]); start++);
    for (; end > start
            && std::isspace(ret[end]); end--);
    return ret.substr(start, end + 1);
}

tokenizer::tokenizer(fs::path filePath) 
    : _filePath(std::move(filePath)) {}

auto tokenizer::tokens(void) const -> std::generator<tokenizer::token> {
    parser p { _filePath };
    auto gen = p.strings();
    auto it = gen.begin();

    tokenizer::token ret {};
    while (it != gen.end()) {
__tokenizer_scan_start:
        ret.strings.clear();
        const auto& str = *it;

        // handle syntax tokens
        if (str.text == ",") {
            ret.type = tokenizer::token_t::COMMA;
            ret.strings.push_back(str);
            co_yield ret;

            it++;
            continue;
        } else if (str.text == "]") {
            ret.type = tokenizer::token_t::CSQARE;
            ret.strings.push_back(str);
            co_yield ret;

            it++;
            continue;
        } else if (str.text == "-") {
            ret.type = tokenizer::token_t::DASH;
            ret.strings.push_back(str);
            co_yield ret;

            it++;
            continue;
        } else if (str.text == "[") {
            ret.type = tokenizer::token_t::OSQARE;
            ret.strings.push_back(str);
            co_yield ret;

            it++;
            continue;
        } else if (str.text == ":") {
            ret.type = tokenizer::token_t::COLON;
            ret.strings.push_back(str);
            co_yield ret;

            it++;
            continue;
        }

        // handle number tokens
        static const std::regex re(
                R"(^[-+]?(?:\d+(?:\.\d*)?|\.\d+)$)");

        if (std::regex_match(str.text, re)) {
            ret.type = tokenizer::token_t::NUMBER;
            ret.strings.push_back(str);
            co_yield ret;

            it++;
            continue;
        }

        // handle multi-line string tokens
        if (str.text == ">" || str.text == "|") {
            if (str.text == ">")
                ret.type = tokenizer::token_t::STRING;
            else if (str.text == "|")
                ret.type = tokenizer::token_t::STRING_LITERAL;
            else throw tokenizer::tokenizer_error(
                    std::format(
                        "Multi-line string definition character not implemented ({:?}): [{}:{}:{}] [{}].",
                        str.text, _filePath.filename().string(),
                        str.row, str.col, _filePath.parent_path().string()));

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
                } else ret.strings.push_back(nextWord);

                it++;
            };

            co_yield ret;

            it++;
            continue;
        }

        // handle UUID tokens
        if (uuid::test_v4(str.text)) {
            ret.type = tokenizer::token_t::UUID;
            ret.strings.push_back(str);
            co_yield ret;

            it++;
            continue;
        }

        // defaults to string token
        ret.type = tokenizer::token_t::STRING;
        ret.strings.push_back(str);
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
