#include "enviroment.hxx"

auto enviroment::get_instance(void) noexcept -> enviroment& {
    static enviroment env;
    return env;
}
