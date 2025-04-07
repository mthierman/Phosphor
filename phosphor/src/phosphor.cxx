#include "phosphor.hxx"
#include "config.hxx"

auto config { phosphor::config() };
auto theme { phosphor::theme::light };

auto phosphor::system_dark_mode() -> bool {
    return pane::color(winrt::UIColorType::Background).is_dark();
}

auto window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
    if (msg == WM_SETTINGCHANGE) {
        config.load();

        auto desktop_wallpaper { wil::CoCreateInstance<IDesktopWallpaper>(CLSID_DesktopWallpaper,
                                                                          CLSCTX_ALL) };
        UINT count;
        LPWSTR monitor;

        desktop_wallpaper->GetMonitorDevicePathCount(&count);
        desktop_wallpaper->GetMonitorDevicePathAt(0, &monitor);

        if (phosphor::system_dark_mode()) {
            theme = phosphor::theme::dark;
        } else {
            theme = phosphor::theme::light;
        }

        switch (theme) {
            case phosphor::theme::dark: {
                pane::debug(config.settings.dark);
                desktop_wallpaper->SetWallpaper(0, config.settings.dark.c_str());
            } break;
            case phosphor::theme::light: {
                pane::debug(config.settings.light);
                desktop_wallpaper->SetWallpaper(0, config.settings.light.c_str());
            } break;
        }
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

auto phosphor::make_window() -> void {
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
    config.load();
    phosphor::make_window();

    return pane::system::message_loop();
}
