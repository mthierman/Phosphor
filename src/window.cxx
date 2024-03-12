#include "window.hxx"
#include "utility.hxx"

namespace Phosphor
{
auto Window::WndProc(::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT
{
    switch (uMsg)
    {
        case WM_CREATE: return on_create(wParam, lParam);
        case WM_SETTINGCHANGE: return on_setting_change(wParam, lParam);
    }

    return ::DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto Window::on_create(::WPARAM wParam, ::LPARAM lParam) -> int
{
    m_wallpaper = wil::CoCreateInstance<IDesktopWallpaper>(CLSID_DesktopWallpaper, CLSCTX_ALL);

    return 0;
}

auto Window::on_setting_change(::WPARAM wParam, ::LPARAM lParam) -> int
{
    auto isDark{is_dark()};

    unsigned int count;
    Phosphor::check(m_wallpaper->GetMonitorDevicePathCount(&count));

    LPWSTR monitor;
    Phosphor::check(m_wallpaper->GetMonitorDevicePathAt(0, &monitor));

    auto pictures{glow::known_folder(FOLDERID_Pictures)};

    auto images{std::make_pair(pictures / "wallpapers" / "dark.jpg",
                               pictures / "wallpapers" / "light.jpg")};

    if (isDark) { m_wallpaper->SetWallpaper(0, images.first.wstring().c_str()); }

    else { m_wallpaper->SetWallpaper(0, images.second.wstring().c_str()); }

    return 0;
}
} // namespace Phosphor
