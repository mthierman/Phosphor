#include "phosphor.hpp"
#include <filesystem>
#include <glaze/glaze.hpp>
#include <pane/pane.hxx>

phosphor::phosphor() {
    this->config.load();
    this->desktop_wallpaper->GetMonitorDevicePathCount(&this->count);
    this->desktop_wallpaper->GetMonitorDevicePathAt(0, &this->monitor);
}

auto phosphor::run() -> int {
    auto self { Self() };

    auto window { pane::window(
        std::move([&self](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
        return self.message_handler(hwnd, msg, wparam, lparam);
    })) };

    return pane::system::message_loop();
};

auto phosphor::message_handler(this Self& self, HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    -> LRESULT {
    if (msg == WM_SETTINGCHANGE) {
        self.config.load();

        if (pane::color(winrt::UIColorType::Background).is_dark()) {
            self.theme = theme::dark;
            self.desktop_wallpaper->SetWallpaper(0, self.config.settings.dark.c_str());
        } else {
            self.theme = theme::light;
            self.desktop_wallpaper->SetWallpaper(0, self.config.settings.light.c_str());
        }

        pane::debug(self.config.settings.dark);
        pane::debug(self.config.settings.light);
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}
