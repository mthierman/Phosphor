#include "phosphor.hpp"
#include <filesystem>
#include <glaze/glaze.hpp>
#include <pane/pane.hxx>

phosphor::phosphor() {
    this->config.load();
    this->desktop_wallpaper->GetMonitorDevicePathCount(&this->count);
    this->desktop_wallpaper->GetMonitorDevicePathAt(0, &this->monitor);
}

auto phosphor::app() -> int {
    auto self { Self() };

    pane::window({ .title { u8"title" }, .visible { false }, .webview { true } },
                 [&self](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
        if (msg == WM_SETTINGCHANGE) {
            self.config.load();

            if (pane::color(winrt::UIColorType::Background).is_dark()) {
                pane::debug(self.config.settings.dark);
                self.theme = theme::dark;
                self.desktop_wallpaper->SetWallpaper(0, self.config.settings.dark.c_str());
            } else {
                pane::debug(self.config.settings.light);
                self.theme = theme::light;
                self.desktop_wallpaper->SetWallpaper(0, self.config.settings.light.c_str());
            }
        }

        return DefWindowProcW(hwnd, msg, wparam, lparam);
    });

    return pane::system::message_loop();
};
