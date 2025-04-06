#include "config.hxx"

namespace phosphor {
config::config() {
    paths.try_emplace(
        u8"config_file",
        pane::filesystem::known_folder()
            .transform([](const std::filesystem::path& path) -> std::filesystem::path {
        return path / u"Phosphor";
    }).value_or(u""));

    paths.try_emplace(
        u8"wallpapers",
        pane::filesystem::known_folder(FOLDERID_Pictures)
            .transform([](const std::filesystem::path& path) -> std::filesystem::path {
        return path / u"Wallpapers";
    }).value_or(u""));
}

auto config::load() -> void { }

auto config::save() -> void { }
} // namespace phosphor
