#include <Windows.h>
#include <ShObjIdl.h>
#include <filesystem>
#include <cstdlib>
#include <utility>
#include <print>
#include <wil/com.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <pane/pane.hxx>
#include <glaze/glaze.hpp>
#include "config.hxx"

namespace winrt {
using namespace winrt::Windows::UI::ViewManagement;
}; // namespace winrt

auto config { phosphor::config() };
auto desktop_wallpaper { wil::CoCreateInstance<IDesktopWallpaper>(CLSID_DesktopWallpaper,
                                                                  CLSCTX_ALL) };
UINT count;
LPWSTR monitor;

auto window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
    if (msg == WM_SETTINGCHANGE) {
        desktop_wallpaper->GetMonitorDevicePathCount(&count);
        desktop_wallpaper->GetMonitorDevicePathAt(0, &monitor);

        switch (pane::color(winrt::UIColorType::Background).is_dark()) {
            case true: {
                // pane::debug(u8"DARK");
                config.theme = phosphor::theme::dark;
            } break;
            case false: {
                // pane::debug(u8"LIGHT");
                config.theme = phosphor::theme::light;
            } break;
        }

        if (auto pictures_directory { pane::filesystem::known_folder(FOLDERID_Pictures) }) {
            auto dark { pictures_directory.value() / u"Wallpapers" / u"dark.png" };
            auto light { pictures_directory.value() / u"Wallpapers" / u"light.png" };

            switch (config.theme) {
                case phosphor::theme::dark: {
                    desktop_wallpaper->SetWallpaper(0, dark.c_str());
                } break;
                case phosphor::theme::light: {
                    desktop_wallpaper->SetWallpaper(0, light.c_str());
                } break;
            }
        }

        std::u8string buffer;
        auto json { glz::write_json(config.theme, buffer) };
        // pane::debug(buffer);
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

auto make_window() -> void {
    auto instance = []() -> HMODULE {
        HMODULE hmodule;

        if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT
                                   | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                               reinterpret_cast<LPCWSTR>(&window_procedure),
                               &hmodule)
            == 0) {
            return nullptr;
        };

        return hmodule;
    }();

    auto icon = []() -> HICON {
        return static_cast<HICON>(
            LoadImageW(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
    }();

    auto cursor = []() -> HCURSOR {
        return static_cast<HCURSOR>(
            LoadImageW(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
    }();

    WNDCLASSEXW window_class { .cbSize { sizeof(WNDCLASSEXW) },
                               .style { 0 },
                               .lpfnWndProc { window_procedure },
                               .cbClsExtra { 0 },
                               .cbWndExtra { 0 },
                               .hInstance { instance },
                               .hIcon { icon },
                               .hCursor { cursor },
                               .hbrBackground { nullptr },
                               .lpszMenuName { nullptr },
                               .lpszClassName { L"MessageWindow" },
                               .hIconSm { icon } };

    if (GetClassInfoExW(instance, window_class.lpszClassName, &window_class) == 0) {
        RegisterClassExW(&window_class);
    }

    [[maybe_unused]] auto window { CreateWindowExW(0,
                                                   window_class.lpszClassName,
                                                   window_class.lpszClassName,
                                                   WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                                                   CW_USEDEFAULT,
                                                   CW_USEDEFAULT,
                                                   CW_USEDEFAULT,
                                                   CW_USEDEFAULT,
                                                   nullptr,
                                                   nullptr,
                                                   window_class.hInstance,
                                                   nullptr) };

    // ShowWindow(window, SW_SHOWNORMAL);
}

auto wWinMain(HINSTANCE /* hinstance */,
              HINSTANCE /* hprevinstance */,
              PWSTR /* pcmdline */,
              int /* ncmdshow */) -> int {
    auto co_init { pane::co_init::apartment_threaded() };

    make_window();

    MSG msg {};
    int r {};

    while ((r = GetMessageW(&msg, nullptr, 0, 0)) != 0) {
        if (r == -1) {
            return EXIT_FAILURE;
        }

        else {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}
