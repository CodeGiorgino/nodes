#include "enviroment.hxx"

namespace fs = std::filesystem;

auto enviroment::get_instance(void) noexcept -> enviroment& {
    static enviroment env;
    return env;
}

auto enviroment::init(std::string_view program) -> void {
    _program = fs::absolute(program);

    const fs::path filePath {
        _program.parent_path() / "assets" / "fonts" / "default.ttf"
    };

    if (!fs::exists(filePath))
        throw std::runtime_error(
                std::format(
                    "enviroment error - cannot find font file: [{}]",
                    filePath.string()));

    _fontDefault
        = _font
        = ::LoadFontEx(filePath.c_str(), fontSizeDefault, NULL, 0);
}

auto enviroment::deinit(void) noexcept -> void {
    ::UnloadFont(_font);
}

auto enviroment::load_font(unsigned fontSize) -> void {
    if (fontSize == _fontSize)
        return;

    const fs::path filePath {
        _program.parent_path() / "assets" / "fonts" / "default.ttf"
    };

    if (!fs::exists(filePath))
        throw std::runtime_error(
                std::format(
                    "enviroment error - cannot find font file: [{}]",
                    filePath.string()));

    _fontSize = fontSize;
    _font = ::LoadFontEx(filePath.c_str(), _fontSize, NULL, 0);
}

auto enviroment::font(void) const noexcept -> std::pair<::Font, unsigned> {
    return std::make_pair(_font, _fontSize);
}

auto enviroment::font_default(void) const noexcept -> std::pair<::Font, unsigned> {
    return std::make_pair(_fontDefault, fontSizeDefault);
}
