#include "config.hxx"

namespace phosphor {
auto config::load(this Self& self) -> void {
    auto config_file { self.paths.at(u8"config_file") };
    auto ec { glz::read_file_json(self.settings,
                                  reinterpret_cast<const char*>(config_file.u8string().data()),
                                  std::string {}) };
}

auto config::save(this const Self& self) -> void {
    auto config_file { self.paths.at(u8"config_file") };
    auto ec { glz::write_file_json(self.settings,
                                   reinterpret_cast<const char*>(config_file.u8string().data()),
                                   std::string {}) };
}
} // namespace phosphor
