#include "phosphor.hxx"
#include "pane/system.hxx"
#include "pane/window.hxx"
#include <glaze/glaze.hpp>
#include <pane/pane.hxx>

namespace phosphor {
auto app::run() -> int {
    auto self { Self() };

    self.config.load();
    self.desktop_wallpaper->GetMonitorDevicePathCount(&self.count);
    self.desktop_wallpaper->GetMonitorDevicePathAt(0, &self.monitor);

    self.settings_window3 = std::make_unique<pane::webview>(
        pane::window_config { .title = u8"settings",
                              .background_color = pane::color { 0, 255, 255, 255 },
                              .visible = true,
                              .shutdown = false },
        pane::webview_config { .home_page = u8"about:blank" });

    return pane::system::message_loop();
}
} // namespace phosphor
