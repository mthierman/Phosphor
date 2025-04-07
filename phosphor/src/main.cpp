#include "phosphor.hpp"

auto config { pane::config<phosphor::settings>() };

auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    auto co_init { pane::co_init::apartment_threaded() };

    phosphor::app();

    return pane::system::message_loop();
}
