#include <Windows.h>
#include <ShObjIdl.h>
#include <filesystem>
#include <utility>

#include <wil/com.h>

#include <glow/glow.hxx>

auto check(HRESULT hr) -> void { std::println("{}", std::system_category().message(hr)); }

struct Window final : public glow::Window
{
    auto WndProc(::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT override;
    auto on_setting_change(::WPARAM wParam, ::LPARAM lParam) -> int;
};

auto Window::WndProc(::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT
{
    switch (uMsg)
    {
        case WM_SETTINGCHANGE: return on_setting_change(wParam, lParam);
    }

    return ::DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto Window::on_setting_change(::WPARAM wParam, ::LPARAM lParam) -> int
{
    auto mode{is_dark()};

    if (mode) { OutputDebugStringA("true!"); }

    return 0;
}

auto main() -> int
{
    glow::CoInitialize coInit;
    glow::App app;
    Window window;
    window.create_window();
    // HRESULT hr;

    // auto wallpaper = wil::CoCreateInstance<IDesktopWallpaper>(CLSID_DesktopWallpaper,
    // CLSCTX_ALL);

    // unsigned int count;
    // check(wallpaper->GetMonitorDevicePathCount(&count));

    // LPWSTR monitor;
    // check(wallpaper->GetMonitorDevicePathAt(0, &monitor));

    // auto pictures{glow::known_folder(FOLDERID_Pictures)};

    // auto images{std::make_pair(pictures / "wallpapers" / "dark.jpg",
    //                            pictures / "wallpapers" / "light.jpg")};

    // check(wallpaper->SetWallpaper(0, images.first.wstring().c_str()));

    return app();
}
