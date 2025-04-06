list(
    APPEND
    CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/libs"
    )
include(pane)
include(download_vcredist)
add_executable(
    phosphor
    WIN32
    )
target_sources(
    phosphor
    PRIVATE "src/phosphor.cxx"
            "src/config.cxx"
            "data/phosphor.rc"
    )
target_link_libraries(
    phosphor
    PRIVATE pane::pane
            pane::config
    )
