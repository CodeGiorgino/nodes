#include <algorithm>
#include <initializer_list>
#include <ranges>
#include <vector>

#include "lexer.hxx"
#include "raylib.h"
#include "tokenizer.hxx"

namespace fs = std::filesystem;
namespace ranges = std::ranges;

lexer::lexer_error::lexer_error(std::string_view message) noexcept {
    _message += message;
}

auto lexer::lexer_error::what(void) const noexcept -> const char* {
    return _message.c_str();
}

lexer::lexer(fs::path filePath)
    : _filePath(std::move(filePath)) {}

auto lexer::lexer::nodes(void) const -> std::generator<node> {
    tokenizer t { _filePath };
    auto gen = t.tokens();
    auto it = gen.begin();

    struct {
        std::string uuid {};
        std::string title {};
        std::string description {};
        ::Vector2 pos {};
    } retBuffer;

    const auto check_token_type =
        [&](std::initializer_list<tokenizer::token_t> types) noexcept -> bool {
            if (it == gen.end())
                return false;
            return ranges::any_of(types,
                    [&](tokenizer::token_t type) noexcept -> bool {
                        return (*it).type == type;
                    });
        };

    const auto expect_token_type =
        [&](std::initializer_list<tokenizer::token_t> types) -> void {
            if (check_token_type(types))
                return;

            std::string expectedTokenTypeNames {};
            for (size_t i = 0; i < types.size(); i++) {
                if (i > 0)
                    expectedTokenTypeNames += ", ";

                const auto& type = types.begin()[i];
                expectedTokenTypeNames += std::format("{:?}",
                        tokenizer::token_type_name(type));
            }

            std::string tokenTypeName = it == gen.end()
                ? "EOF"
                : tokenizer::token_type_name((*it).type).data();

            throw lexer::lexer_error(
                    std::format(
                        "Expected token of type {} but got {:?} instead: [{}:{}:{}].",
                        expectedTokenTypeNames, tokenTypeName,
                        _filePath.string(), (*it).strings[0].row,
                        (*it).strings[0].col));
        };

    const auto check_token_text =
        [&](std::string_view text) noexcept -> bool {
            if (!check_token_type({ tokenizer::token_t::STRING,
                        tokenizer::token_t::STRING_LITERAL }))
                return false;
            return (*it).text() == text;
        };

    const auto expect_token_text =
        [&](std::string_view text) -> void {
            if (check_token_text(text))
                return;

            throw lexer::lexer_error(
                    std::format(
                        "Expected text {:?} but got {:?} instead: [{}:{}:{}].",
                        text, (*it).text(), _filePath.string(),
                        (*it).strings[0].row, (*it).strings[0].col));
        };

    const auto lex_value =
        [&](std::string_view key, std::initializer_list<tokenizer::token_t> types) -> std::string {
            expect_token_text(key);
            it++;

            expect_token_type({ tokenizer::token_t::COLON });
            it++;

            expect_token_type(types);
            const auto retValue = (*it).text();
            it++;
            return retValue;
        };

    const auto lex_list =
        [&](std::string_view key, tokenizer::token_t type) -> std::vector<std::string> {
            expect_token_text(key);
            it++;

            expect_token_type({ tokenizer::token_t::COLON });
            it++;

            std::vector<std::string> retList {};

            tokenizer::token_t delimiter { tokenizer::token_t::COMMA };
            if (check_token_type({ tokenizer::token_t::OSQARE }))
                it++;
            else if (check_token_type({ tokenizer::token_t::DASH }))
                delimiter = tokenizer::token_t::DASH;
            else throw lexer::lexer_error(
                    std::format(
                        "Expected an array definition, but got {:?} instead: [{}:{}:{}].",
                        (*it).text(), _filePath.string(), (*it).strings[0].row,
                        (*it).strings[0].col));

            while (it != gen.end()) {
                if (delimiter == tokenizer::token_t::COMMA) {
                    if (check_token_type({ tokenizer::token_t::CSQARE })) {
                        it++;
                        break;
                    }

                    expect_token_type({ type });
                    retList.emplace_back((*it).text());
                    it++;

                    if (!check_token_type({ tokenizer::token_t::COMMA }))
                        expect_token_type({ tokenizer::token_t::CSQARE });
                    else it++;
                } else if (delimiter == tokenizer::token_t::DASH) {
                    expect_token_type({ tokenizer::token_t::DASH });
                    it++;

                    expect_token_type({ type });
                    retList.emplace_back((*it).text());
                    it++;

                    if (!check_token_type({ tokenizer::token_t::DASH }))
                        break;
                } else throw lexer::lexer_error(
                        std::format(
                            "Lexing an array delimited by {:?} is not implemented: [{}:{}:{}].",
                            (*it).text(), _filePath.string(),
                            (*it).strings[0].row, (*it).strings[0].col));
            };

            return retList;
        };

    while (it != gen.end()) {
        // uuid
        expect_token_type({ tokenizer::token_t::UUID });
        retBuffer.uuid = (*it).text();
        it++;

        expect_token_type({ tokenizer::token_t::COLON });
        it++;

        // title
        retBuffer.title = lex_value("title", { tokenizer::token_t::STRING,
                tokenizer::token_t::STRING_LITERAL });

        // description
        if (check_token_text("description")) {
            retBuffer.description = lex_value("description", {
                        tokenizer::token_t::STRING,
                        tokenizer::token_t::STRING_LITERAL
                    });
        }

        // position
        const auto pos = lex_list("position", { tokenizer::token_t::NUMBER });
        if (pos.size() != 2)
            throw lexer::lexer_error(
                    std::format(
                        "Position expexts 2 entries, but got {} instead: [{}:{}:{}].",
                        pos.size(), _filePath.string(), (*it).strings[0].row,
                        (*it).strings[0].col));

        retBuffer.pos = ::Vector2 {
            std::stof(std::string { pos[0] }),
            std::stof(std::string { pos[1] }),
        };

        auto retNode = node(retBuffer.uuid, retBuffer.title,
                retBuffer.description, retBuffer.pos);

        if (check_token_text("connections")) {
            auto& connections = retNode.connections();
            for (const auto& conn :
                    lex_list("connections", { tokenizer::token_t::UUID }))
                connections.emplace_back(conn);
        }

        co_yield retNode;
    };
}
