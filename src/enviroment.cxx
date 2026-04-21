#include "enviroment.hxx"

namespace fs = std::filesystem;

auto enviroment::get_instance(void)
    noexcept -> enviroment& {
        static enviroment env;
        return env;
    }

auto enviroment::init(std::string_view program)
    -> void {
        _program = fs::absolute(program);

        const auto filePath {
            _program.parent_path() / "assets" / "fonts" / "default.ttf"
        };

        if (!fs::exists(filePath))
            throw std::runtime_error(
                    std::format(
                        "enviroment error: cannot find default font file: [{}]",
                        filePath.string()));

        _font = ::LoadFontEx(filePath.c_str(), fontSize, NULL, 0);
    }

auto enviroment::deinit(void)
    noexcept -> void {
        ::UnloadFont(_font);
    }
