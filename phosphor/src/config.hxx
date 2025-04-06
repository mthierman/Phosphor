#pragma once
#include <unordered_map>
#include <filesystem>
#include <glaze/glaze.hpp>
#include <pane/pane.hxx>

namespace phosphor {
enum struct theme { light = 0, dark };
// Register as strings for glaze
// template <> struct glz::meta<theme> {
//     using enum theme;
//     static constexpr auto value = enumerate(light, dark);
// };

struct settings final {
    phosphor::theme theme { phosphor::theme::dark };
};

struct config final {
    using Self = config;

    config();
    ~config() = default;

    config(const Self& color) = default;
    auto operator=(const Self& color) -> Self& = default;
    config(Self&& color) noexcept = default;
    auto operator=(Self&& color) noexcept -> Self& = default;

    auto load(this const Self& self) -> void;
    // auto save(this const Self& self) -> void;

    std::unordered_map<std::u8string, std::filesystem::path> paths;
    phosphor::settings settings;
};
} // namespace phosphor
