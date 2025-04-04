# file( DOWNLOAD "https://aka.ms/vs/17/release/vc_redist.x64.exe"
# "${PROJECT_BINARY_DIR}/vc_redist.x64.exe" )
list(
    APPEND
    CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/libs"
    )
include(config)
include(webview2)
include(wil)
include(cppwinrt)
include(ada)
include(glaze)
add_executable(
    phosphor
    WIN32
    )
target_sources(
    phosphor
    PRIVATE "src/win32.cxx"
            "data/phosphor.rc"
    )
target_link_libraries(
    phosphor
    PRIVATE phosphor::config
            dwmapi
            gdiplus
            icu
            Urlmon
            WindowsApp
            ada
            glaze
            webview2
            wil
            cppwinrt
    )
add_executable(phosphor_console)
target_sources(
    phosphor_console
    PRIVATE "src/console.cxx"
            "data/phosphor.rc"
    )
target_link_libraries(
    phosphor_console
    PRIVATE phosphor::config
            dwmapi
            gdiplus
            icu
            Urlmon
            WindowsApp
            ada
            glaze
            webview2
            wil
            cppwinrt
    )
