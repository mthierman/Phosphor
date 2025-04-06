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

auto config::load(this const Self& self) -> void {
    phosphor::settings settings;

    std::string buffer;

    auto ec { glz::read_file_json(
        settings,
        reinterpret_cast<const char*>(self.paths.at(u8"config_file").u8string().data()),
        buffer) };

    pane::debug(buffer.data());
}

auto config::save(this const Self& self) -> void { }
} // namespace phosphor
