#include <Windows.h>
#include <cstdlib>
#include <expected>
#include <system_error>

auto last_error() -> std::error_code {
    return std::error_code(GetLastError(), std::system_category());
}

auto module_handle() -> std::expected<HMODULE, std::error_code> {
    HMODULE hmodule;

    if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT
                               | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                           reinterpret_cast<LPCWSTR>(&module_handle),
                           &hmodule)
        == 0) {
        return std::unexpected(last_error());
    };

    return hmodule;
}

auto application_icon() -> HICON {
    return static_cast<HICON>(
        LoadImageW(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto arrow_cursor() -> HCURSOR {
    return static_cast<HCURSOR>(
        LoadImageW(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE));
}

auto CALLBACK window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
    // if (msg == WM_NCCREATE) {
    //     auto create { reinterpret_cast<CREATESTRUCTW*>(lparam) };

    //     if (auto self { static_cast<Self*>(create->lpCreateParams) }; self) {
    //         SetWindowLongPtrW(hwnd, 0, reinterpret_cast<LONG_PTR>(self));
    //         self->window_handle = hwnd;
    //     }
    // }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

auto wmain(int /* argc */, wchar_t* /* argv */[], wchar_t* /* envp */[]) -> int {
    WNDCLASSEXW window_class { .cbSize { sizeof(WNDCLASSEXW) },
                               .style { 0 },
                               .lpfnWndProc { window_procedure },
                               .cbClsExtra { 0 },
                               .cbWndExtra { 0 },
                               .hInstance { module_handle().value_or(nullptr) },
                               .hIcon { application_icon() },
                               .hCursor { arrow_cursor() },
                               .hbrBackground { nullptr },
                               .lpszMenuName { nullptr },
                               .lpszClassName { L"Window" },
                               .hIconSm { application_icon() } };

    HWND window_handle;

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
