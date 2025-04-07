#include "phosphor.hpp"
#include <filesystem>
#include <glaze/glaze.hpp>
#include <pane/pane.hxx>

auto phosphor::run(this Self& self) -> int {
    self.config.load();
    self.desktop_wallpaper->GetMonitorDevicePathCount(&self.count);
    self.desktop_wallpaper->GetMonitorDevicePathAt(0, &self.monitor);

    self.window = pane::window(
        std::move([&self](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
        if (msg == WM_SETTINGCHANGE) {
            self.config.load();

            if (pane::color(winrt::UIColorType::Background).is_dark()) {
                self.theme = phosphor::theme::dark;
                self.desktop_wallpaper->SetWallpaper(0, self.config.settings.dark.c_str());
            } else {
                self.theme = phosphor::theme::light;
                self.desktop_wallpaper->SetWallpaper(0, self.config.settings.light.c_str());
            }

            pane::debug(self.config.settings.dark);
            pane::debug(self.config.settings.light);
        }

        return DefWindowProcW(hwnd, msg, wparam, lparam);
    }));

    self.window.activate();

    return pane::system::message_loop();
};
