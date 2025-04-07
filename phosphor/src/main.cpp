#include "phosphor.hpp"

auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    // auto phosphor { phosphor::init() };

    return phosphor::run();
}
