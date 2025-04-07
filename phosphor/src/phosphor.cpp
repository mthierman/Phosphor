#include "phosphor.hpp"
#include <filesystem>
#include <glaze/glaze.hpp>
#include <pane/pane.hxx>

namespace phosphor {
app::app() {
    config.load();

    auto wndproc = [this](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
        if (msg == WM_SETTINGCHANGE) {
            this->config.load();

            this->desktop_wallpaper->GetMonitorDevicePathCount(&this->count);
            this->desktop_wallpaper->GetMonitorDevicePathAt(0, &this->monitor);

            if (this->system_dark_mode()) {
                this->theme = phosphor::theme::dark;
            } else {
                this->theme = phosphor::theme::light;
            }

            switch (theme) {
                case phosphor::theme::dark: {
                    pane::debug(this->config.settings.dark);
                    desktop_wallpaper->SetWallpaper(0, config.settings.dark.c_str());
                } break;
                case phosphor::theme::light: {
                    pane::debug(this->config.settings.light);
                    desktop_wallpaper->SetWallpaper(0, config.settings.light.c_str());
                } break;
            }
        }

        return DefWindowProcW(hwnd, msg, wparam, lparam);
    };

    window = std::make_unique<pane::window>(wndproc);
};

auto app::system_dark_mode() -> bool {
    return pane::color(winrt::UIColorType::Background).is_dark();
}
} // namespace phosphor
