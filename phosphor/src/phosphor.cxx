#include "phosphor.hxx"
#include <filesystem>
#include <glaze/glaze.hpp>
#include <pane/pane.hxx>

namespace phosphor {
auto app::run() -> int {
    auto self { Self() };

    self.config.load();
    self.desktop_wallpaper->GetMonitorDevicePathCount(&self.count);
    self.desktop_wallpaper->GetMonitorDevicePathAt(0, &self.monitor);

    return pane::system::message_loop();
};
} // namespace phosphor
