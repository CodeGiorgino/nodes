#include "enviroment.hxx"

namespace fs = std::filesystem;

enviroment::enviroment_error::enviroment_error(std::string_view message) noexcept {
    _message += message;
}

auto enviroment::enviroment_error::what(void) const noexcept -> const char* {
    return _message.c_str();
}

auto enviroment::get_instance(void) noexcept -> enviroment& {
    static enviroment env;
    return env;
}

auto enviroment::init(std::string_view program, fs::path configFilePath)
    -> void {
        _program = fs::absolute(program);
        _configFilePath = configFilePath;

        // load the default font
        load_font();
    }

auto enviroment::deinit(void) noexcept -> void {
    for (auto& [k, v] : _fontMap)
        ::UnloadFont(v);
    _fontMap.clear();
}

auto enviroment::load_font(unsigned fontSize) -> void {
    _fontSize = fontSize;

    if (_fontMap.contains(fontSize))
        return;

    const fs::path filePath {
        _program.parent_path() / "assets" / "fonts" / "default.ttf"
    };

    if (!fs::exists(filePath))
        throw enviroment::enviroment_error(
                std::format(
                    "Cannot find font file: [{}].",
                    filePath.string()));

    _fontMap.insert({
                fontSize,
                ::LoadFontEx(filePath.c_str(), _fontSize, NULL, 0)
            });
}

auto enviroment::font(void) const -> std::pair<::Font, unsigned> {
    if (auto search = _fontMap.find(_fontSize);
            search != _fontMap.end())
        return std::make_pair(search->second, _fontSize);

    throw enviroment::enviroment_error(
            std::format("Font size not loaded: {}.",
                _fontSize));
}

auto enviroment::font_default(void) const -> std::pair<::Font, unsigned> {
    if (auto search = _fontMap.find(fontSizeDefault);
            search != _fontMap.end())
        return std::make_pair(search->second, fontSizeDefault);

    throw enviroment::enviroment_error("Default font not loaded.");
}
