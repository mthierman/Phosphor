#include "phosphor.hpp"
#include <filesystem>
#include <glaze/glaze.hpp>
#include <pane/pane.hxx>

namespace phosphor {
app::app() {
    config.load();

    auto wndproc = [](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
        if (msg == WM_SETTINGCHANGE) {
            // config.load();

            auto desktop_wallpaper { wil::CoCreateInstance<IDesktopWallpaper>(
                CLSID_DesktopWallpaper, CLSCTX_ALL) };
            UINT count;
            LPWSTR monitor;

            desktop_wallpaper->GetMonitorDevicePathCount(&count);
            desktop_wallpaper->GetMonitorDevicePathAt(0, &monitor);

            if (phosphor::system_dark_mode()) {
                // theme = phosphor::theme::dark;
            } else {
                // theme = phosphor::theme::light;
            }

            // switch (theme) {
            //     case phosphor::theme::dark: {
            //         pane::debug(config.settings.dark);
            //         desktop_wallpaper->SetWallpaper(0, config.settings.dark.c_str());
            //     } break;
            //     case phosphor::theme::light: {
            //         pane::debug(config.settings.light);
            //         desktop_wallpaper->SetWallpaper(0, config.settings.light.c_str());
            //     } break;
            // }
        }

        return DefWindowProcW(hwnd, msg, wparam, lparam);
    };

    window = std::make_unique<pane::window>(wndproc);
};
auto system_dark_mode() -> bool { return pane::color(winrt::UIColorType::Background).is_dark(); }
} // namespace phosphor
