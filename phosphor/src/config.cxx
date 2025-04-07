#include "config.hxx"

namespace phosphor {
auto config::load(this Self& self) -> void {
    if (self.config_file.empty()) {
        return;
    }

    auto ec { glz::read_file_json(self.settings,
                                  reinterpret_cast<const char*>(self.config_file.u8string().data()),
                                  std::string {}) };
}

auto config::save(this const Self& self) -> void {
    if (self.config_file.empty()) {
        return;
    }

    auto ec { glz::write_file_json(
        self.settings,
        reinterpret_cast<const char*>(self.config_file.u8string().data()),
        std::string {}) };
}
} // namespace phosphor
