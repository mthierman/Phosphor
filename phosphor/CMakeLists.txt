# file( DOWNLOAD "https://aka.ms/vs/17/release/vc_redist.x64.exe"
# "${PROJECT_BINARY_DIR}/vc_redist.x64.exe" )
list(
    APPEND
    CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/libs"
    )
include(pane)
add_executable(
    phosphor
    WIN32
    )
target_sources(
    phosphor
    PRIVATE "src/phosphor.cxx"
            "data/phosphor.rc"
    )
target_link_libraries(
    phosphor
    PRIVATE pane::pane
            pane::config
    )
