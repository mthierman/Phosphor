#include "phosphor.hpp"
#include <filesystem>
#include <glaze/glaze.hpp>
#include <pane/pane.hxx>

namespace phosphor {
app::app() {
    config.load();

    this->desktop_wallpaper->GetMonitorDevicePathCount(&this->count);
    this->desktop_wallpaper->GetMonitorDevicePathAt(0, &this->monitor);

    auto wndproc = [this](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
        if (msg == WM_SETTINGCHANGE) {
            this->config.load();

            if (pane::color(winrt::UIColorType::Background).is_dark()) {
                this->theme = phosphor::theme::dark;
                desktop_wallpaper->SetWallpaper(0, config.settings.dark.c_str());
            } else {
                this->theme = phosphor::theme::light;
                desktop_wallpaper->SetWallpaper(0, config.settings.light.c_str());
            }

            pane::debug(this->config.settings.dark);
            pane::debug(this->config.settings.light);
        }

        return DefWindowProcW(hwnd, msg, wparam, lparam);
    };

    window = std::make_unique<pane::window>(std::move(wndproc));
};
} // namespace phosphor
