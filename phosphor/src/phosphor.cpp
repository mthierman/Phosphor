#include "phosphor.hpp"
#include <filesystem>
#include <glaze/glaze.hpp>
#include <pane/pane.hxx>

auto phosphor::app() -> int {
    auto self { Self() };

    self.config.load();
    self.desktop_wallpaper->GetMonitorDevicePathCount(&self.count);
    self.desktop_wallpaper->GetMonitorDevicePathAt(0, &self.monitor);

    pane::window(
        pane::window::config {
            .title { u8"title" }, .visible { false }, .webview { true }, .home_page { u8"" } },
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
