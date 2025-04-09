#pragma once
#include <Windows.h>
#include <ShObjIdl.h>
#include <filesystem>
#include <cstdlib>
#include <memory>
#include <utility>
#include <print>
#include <wil/com.h>
#include <glaze/glaze.hpp>
#include <pane/pane.hxx>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>

namespace phosphor {
enum struct theme { light = 0, dark };

struct app final {
    using Self = app;

    struct settings final {
        std::filesystem::path dark;
        std::filesystem::path light;
    };

    ~app() = default;

    app(const Self& self) = delete;
    auto operator=(const Self& self) -> Self& = delete;
    app(Self&& self) noexcept = delete;
    auto operator=(Self&& self) noexcept -> Self& = delete;

    static auto run() -> int;

    pane::co_init co_init { pane::co_init::apartment_threaded() };
    pane::config<phosphor::app::settings> config { pane::config<phosphor::app::settings>() };
    phosphor::theme theme { phosphor::theme::light };
    wil::com_ptr<IDesktopWallpaper> desktop_wallpaper { wil::CoCreateInstance<IDesktopWallpaper>(
        CLSID_DesktopWallpaper, CLSCTX_ALL) };
    UINT count;
    LPWSTR monitor;
    std::filesystem::path config_file {
        pane::filesystem::known_folder()
            .transform([](const std::filesystem::path& path) -> std::filesystem::path {
        return path / u"Phosphor" / u"config.json";
    }).value_or(u"")
    };

private:
    app() = default;
};
} // namespace phosphor

template <> struct glz::meta<phosphor::theme> {
    using enum phosphor::theme;
    static constexpr auto value { glz::enumerate(light, dark) };
};
