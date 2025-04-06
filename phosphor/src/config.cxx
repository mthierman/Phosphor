#include "config.hxx"

namespace phosphor {
config::config() {
    paths.try_emplace(
        u8"config_file",
        pane::filesystem::known_folder()
            .transform([](const std::filesystem::path& path) -> std::filesystem::path {
        return path / u"Phosphor" / u"config.json";
    }).value_or(u""));

    paths.try_emplace(
        u8"wallpapers",
        pane::filesystem::known_folder(FOLDERID_Pictures)
            .transform([](const std::filesystem::path& path) -> std::filesystem::path {
        return path / u"Wallpapers";
    }).value_or(u""));
}

auto config::load(this Self& self) -> void {
    phosphor::settings settings;
    auto config_file { self.paths.at(u8"config_file") };
    auto ec { glz::read_file_json(
        settings, reinterpret_cast<const char*>(config_file.u8string().data()), std::string {}) };

    self.settings = settings;
}

auto config::save(this const Self& self) -> void {
    auto config_file { self.paths.at(u8"config_file") };
    auto ec { glz::write_file_json(self.settings,
                                   reinterpret_cast<const char*>(config_file.u8string().data()),
                                   std::string {}) };
}
} // namespace phosphor
