#include "phosphor.hxx"
#include "config.hxx"

auto config { phosphor::config() };
auto theme { phosphor::theme::light };

auto window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
    if (msg == WM_SETTINGCHANGE) {
        config.load();

        auto desktop_wallpaper { wil::CoCreateInstance<IDesktopWallpaper>(CLSID_DesktopWallpaper,
                                                                          CLSCTX_ALL) };
        UINT count;
        LPWSTR monitor;

        desktop_wallpaper->GetMonitorDevicePathCount(&count);
        desktop_wallpaper->GetMonitorDevicePathAt(0, &monitor);

        if (phosphor::system_dark_mode()) {
            theme = phosphor::theme::dark;
        } else {
            theme = phosphor::theme::light;
        }

        switch (theme) {
            case phosphor::theme::dark: {
                pane::debug(config.settings.dark);
                desktop_wallpaper->SetWallpaper(0, config.settings.dark.c_str());
            } break;
            case phosphor::theme::light: {
                pane::debug(config.settings.light);
                desktop_wallpaper->SetWallpaper(0, config.settings.light.c_str());
            } break;
        }
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    auto co_init { pane::co_init::apartment_threaded() };
    config.load();
    phosphor::make_window();

    return pane::system::message_loop();
}
