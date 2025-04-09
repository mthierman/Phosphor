#include "phosphor.hpp"
#include <filesystem>
#include <glaze/glaze.hpp>
#include <pane/pane.hxx>

namespace phosphor {
auto app::run() -> int {
    auto self { Self() };

    self.config.load();
    self.desktop_wallpaper->GetMonitorDevicePathCount(&self.count);
    self.desktop_wallpaper->GetMonitorDevicePathAt(0, &self.monitor);

    auto settings_window { pane::window(
        { .title { u8"settings" },
          .background_color { pane::color { 255, 255, 255, 255 } },
          .visible { true },
          .webview { false } },
        [&self](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
        if (auto window { pane::window::get_instance(hwnd) }) {
            if (msg == WM_ERASEBKGND) {
                auto rect { window->client_rect() };
                FillRect(reinterpret_cast<HDC>(wparam),
                         &rect,
                         pane::color(255, 0, 255, 255).to_hbrush());

                return 1;
            }
        }

        return DefWindowProcW(hwnd, msg, wparam, lparam);
    }) };

    auto main_window { pane::window(
        {
            .title { u8"phosphor" },
            .background_color { pane::color {} },
            .visible { false },
            .webview { false },
        },
        [&self](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
        if (msg == WM_SETTINGCHANGE) {
            self.config.load();

            if (pane::color(winrt::Windows::UI::ViewManagement::UIColorType::Background)
                    .is_dark()) {
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
    }) };

    return pane::system::message_loop();
};
} // namespace phosphor
