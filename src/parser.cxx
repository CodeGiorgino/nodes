#include <format>
#include <fstream>

#include "parser.hxx"

namespace fs = std::filesystem;

parser::parser_error::parser_error(std::string_view message) noexcept {
    _message += message;
}

auto parser::parser_error::what(void) const noexcept -> const char* {
    return _message.c_str();
}

parser::parser(fs::path filePath)
    : _filePath(std::move(filePath)) {
        if (!fs::exists(_filePath))
            throw parser::parser_error(
                    std::format(
                        "Cannot find file: [{}].",
                        _filePath.string()));
        else if (!fs::is_regular_file(_filePath))
            throw parser::parser_error(
                    std::format(
                        "Cannot read non-regular file: [{}].",
                        _filePath.string()));
    }

auto parser::words(void) const -> std::generator<parser::word> {
    std::ifstream ifs(_filePath.string());
    if (!ifs) throw parser::parser_error(
            std::format(
                "Cannot open file: [{}].",
                _filePath.string()));

    parser::word ret;
    while (!ifs.eof()) {
        // update the indentation level
        if (ret.col == 0) {
            while (!ifs.eof()
                    && (char)(unsigned)ifs.peek() == ' ') {
                ifs.get();
                ret.col++;
                ret.indentLevel++;
            };
        }

        const char ch = (unsigned)ifs.get();
        if (ch == '#') { // comments
            while (!ifs.eof()
                    && (char)(unsigned)ifs.peek() != '\n') {
                ifs.get();
                ret.col++;
            };

            if (!ret.text.empty()) {
                co_yield ret;
                ret.text.clear();
            }
        } else if (ch == '\n') {
            if (!ret.text.empty()) {
                co_yield ret;
                ret.text.clear();
            }

            ret.row++;
            ret.col = 0;
            ret.indentLevel = 0;
        } else if (std::isspace(ch)) {
            if (!ret.text.empty()) {
                co_yield ret;
                ret.text.clear();
            }
        } else if (!std::isalnum(ch)) {
            if (ch == EOF)
                break;
            else if (!ret.text.empty()) {
                if (ch == '-') {
                    ret.text += ch;
                    ret.col++;
                    continue;
                }

                co_yield ret;
                ret.text.clear();
            }

            ret.text = ch;
            ret.col++;
            co_yield ret;
            ret.text.clear();
        } else {
            ret.text += ch;
            ret.col++;
        }
    };

    if (!ret.text.empty())
        co_yield ret;
}
