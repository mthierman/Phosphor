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

namespace winrt {
using namespace winrt::Windows::UI::ViewManagement;
}; // namespace winrt

enum struct theme { light = 0, dark };

struct phosphor final {
    using Self = phosphor;

    struct settings final {
        std::filesystem::path dark;
        std::filesystem::path light;
    };

    ~phosphor() = default;

    phosphor(const Self& self) = delete;
    auto operator=(const Self& self) -> Self& = delete;
    phosphor(Self&& self) noexcept = delete;
    auto operator=(Self&& self) noexcept -> Self& = delete;

    static auto run() -> int;

    pane::co_init co_init { pane::co_init::apartment_threaded() };
    pane::config<phosphor::settings> config { pane::config<phosphor::settings>() };
    theme theme { theme::light };
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
    pane::window window;

private:
    phosphor() = default;
};

template <> struct glz::meta<theme> {
    using enum theme;
    static constexpr auto value { glz::enumerate(light, dark) };
};
