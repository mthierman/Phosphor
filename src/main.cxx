#include <Windows.h>
#include <ShObjIdl.h>
#include <filesystem>
#include <utility>

#include <wil/com.h>

#include <glow/glow.hxx>

auto check(HRESULT hr) -> void { std::println("{}", std::system_category().message(hr)); }

auto main() -> int
{
    glow::CoInitialize coInit;
    HRESULT hr;

    auto wallpaper = wil::CoCreateInstance<IDesktopWallpaper>(CLSID_DesktopWallpaper, CLSCTX_ALL);

    unsigned int count;
    check(wallpaper->GetMonitorDevicePathCount(&count));

    LPWSTR monitor;
    check(wallpaper->GetMonitorDevicePathAt(0, &monitor));

    auto pictures{glow::known_folder(FOLDERID_Pictures)};

    auto images{std::make_pair(pictures / "wallpapers" / "dark.jpg",
                               pictures / "wallpapers" / "light.jpg")};

    check(wallpaper->SetWallpaper(0, images.first.wstring().c_str()));
}
