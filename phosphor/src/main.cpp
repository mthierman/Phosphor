#include "phosphor.hpp"

auto config { pane::config<phosphor::settings>() };

auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    auto app { phosphor::app() };

    return app.run();
}
