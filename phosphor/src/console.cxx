#include <Windows.h>
#include <print>
#include <cstdlib>

auto CALLBACK window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
    // if (msg == WM_NCCREATE) {
    //     auto create { reinterpret_cast<CREATESTRUCTW*>(lparam) };

    //     if (auto self { static_cast<Self*>(create->lpCreateParams) }; self) {
    //         SetWindowLongPtrW(hwnd, 0, reinterpret_cast<LONG_PTR>(self));
    //         self->window_handle = hwnd;
    //     }
    // }

    if (msg == WM_SETTINGCHANGE) {
        std::println("WM_SETTINGCHANGE");
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

auto wmain(int /* argc */, wchar_t* /* argv */[], wchar_t* /* envp */[]) -> int {
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
