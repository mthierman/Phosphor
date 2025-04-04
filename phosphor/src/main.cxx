#include <Windows.h>
#include <wil/com.h>
#include <print>
#include <system_error>
#include <filesystem>
#include <utility>

// https://stackoverflow.com/questions/8366577/thoughts-on-a-different-way-to-running-a-win32-event-loop-without-wndproc
// https://stackoverflow.com/questions/63614685/why-does-the-win32-message-loop-stop-working-when-threaded

struct Phosphor final : public glow::Window {
    auto WndProc(::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT override;
    auto on_create(::WPARAM wParam, ::LPARAM lParam) -> int;
    auto on_setting_change(::WPARAM wParam, ::LPARAM lParam) -> int;

    auto check(::HRESULT hr) -> void;

    glow::CoInitialize m_coInit;
    wil::com_ptr<IDesktopWallpaper> m_wallpaper;
    bool m_isDark;
};

auto Phosphor::WndProc(::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT {
    switch (uMsg) {
        case WM_CREATE:
            return on_create(wParam, lParam);
        case WM_SETTINGCHANGE:
            return on_setting_change(wParam, lParam);
    }

    return ::DefWindowProcA(m_hwnd.get(), uMsg, wParam, lParam);
}

auto Phosphor::on_create(::WPARAM wParam, ::LPARAM lParam) -> int {
    m_wallpaper = wil::CoCreateInstance<IDesktopWallpaper>(CLSID_DesktopWallpaper, CLSCTX_ALL);
    m_isDark = is_dark();

    return 0;
}

auto Phosphor::on_setting_change(::WPARAM wParam, ::LPARAM lParam) -> int {
    if (m_isDark == is_dark()) {
        return 0;
    }

    else {
        m_isDark = is_dark();

        unsigned int count;
        check(m_wallpaper->GetMonitorDevicePathCount(&count));

        LPWSTR monitor;
        check(m_wallpaper->GetMonitorDevicePathAt(0, &monitor));

        auto pictures { glow::known_folder(FOLDERID_Pictures) };

        auto images { std::make_pair(pictures / "wallpapers" / "dark.png",
                                     pictures / "wallpapers" / "light.png") };

        if (!std::filesystem::exists(images.first)) {
            images.first = pictures / "wallpapers" / "dark.jpg";
        }

        if (!std::filesystem::exists(images.second)) {
            images.second = pictures / "wallpapers" / "light.jpg";
        }

        if (m_isDark) {
            if (std::filesystem::exists(images.first)) {
                m_wallpaper->SetWallpaper(0, images.first.wstring().c_str());
            }
        }

        else {
            if (std::filesystem::exists(images.second)) {
                m_wallpaper->SetWallpaper(0, images.second.wstring().c_str());
            }
        }
    }

    return 0;
}

auto Phosphor::check(::HRESULT hr) -> void {
    std::println("{}", std::system_category().message(hr));
}

auto wmain(int /* argc */, wchar_t* /* argv */[], wchar_t* /* envp */[]) -> int {
    glow::App app;

    Phosphor phosphor;
    phosphor.create_window();

    return app();
}
