#pragma once
#include <unordered_map>
#include <filesystem>
#include <glaze/glaze.hpp>
#include <pane/pane.hxx>

namespace phosphor {
enum struct theme { light = 0, dark };
}

template <> struct glz::meta<phosphor::theme> {
    using enum phosphor::theme;
    static constexpr auto value = glz::enumerate(light, dark);
};

namespace phosphor {
struct settings final {
    std::filesystem::path dark;
    std::filesystem::path light;
};

struct config final {
    using Self = config;

    config() = default;
    ~config() = default;

    config(const Self& color) = delete;
    auto operator=(const Self& color) -> Self& = delete;
    config(Self&& color) noexcept = delete;
    auto operator=(Self&& color) noexcept -> Self& = delete;

    auto load(this Self& self) -> void;
    auto save(this const Self& self) -> void;

    std::filesystem::path config_file {
        pane::filesystem::known_folder()
            .transform([](const std::filesystem::path& path) -> std::filesystem::path {
        return path / u"Phosphor" / u"config.json";
    }).value_or(u"")
    };

    phosphor::settings settings;
    phosphor::theme theme { phosphor::theme::light };
};
} // namespace phosphor
