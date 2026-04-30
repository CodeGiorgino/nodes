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
                        "Cannot find file: [{}] [{}].",
                        _filePath.filename().string(),
                        _filePath.parent_path().string()));
        else if (!fs::is_regular_file(_filePath))
            throw parser::parser_error(
                    std::format(
                        "Cannot read non-regular file: [{}] [{}].",
                        _filePath.filename().string(),
                        _filePath.parent_path().string()));
    }

auto parser::strings(void) const -> std::generator<parser::string> {
    std::ifstream ifs(_filePath.string());
    if (!ifs) throw parser::parser_error(
            std::format(
                "Cannot open file: [{}] [{}].",
                _filePath.filename().string(),
                _filePath.parent_path().string()));

    parser::string ret;
    const auto trim =
        [&](void) noexcept -> void {
            if (ret.text.empty())
                return;

            size_t start = 0;
            size_t end = ret.text.size() - 1;
            for (; start < ret.text.size()
                    && std::isspace(ret.text[start]); start++);
            for (; end > start
                    && std::isspace(ret.text[end]); end--);
            ret.text = ret.text.substr(start, end + 1);
        };

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

            trim();
            if (!ret.text.empty()) {
                co_yield ret;
                ret.text.clear();
            }
        } else if (ch == '\n') {
            trim();
            if (!ret.text.empty()) {
                co_yield ret;
                ret.text.clear();
            }

            ret.row++;
            ret.col = 0;
            ret.indentLevel = 0;
        } else if (!std::isspace(ch)
                && !std::isalnum(ch)) {
            if (ch == EOF)
                break;
            else if (!ret.text.empty()) {
                if (ch == '-') {
                    ret.text += ch;
                    ret.col++;
                    continue;
                }

                trim();
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

    trim();
    if (!ret.text.empty())
        co_yield ret;
}
