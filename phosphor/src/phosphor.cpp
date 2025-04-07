#include "phosphor.hpp"
#include <filesystem>
#include <glaze/glaze.hpp>
#include <pane/pane.hxx>

auto phosphor::system_dark_mode() -> bool {
    return pane::color(winrt::UIColorType::Background).is_dark();
}

auto phosphor::make_window(WNDPROC window_procedure) -> void {
    auto instance = []() -> HMODULE {
        HMODULE hmodule;

        if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT
                                   | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                               reinterpret_cast<LPCWSTR>(&phosphor::make_window),
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
