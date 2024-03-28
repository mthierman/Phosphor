cmake_minimum_required(VERSION 3.28)

project(
    Phosphor
    VERSION 0.0.1
    LANGUAGES C
              CXX
)

enable_testing()

include(FetchContent)

add_subdirectory(libs/Glow)

include(libs/Glow/cmake/ReleaseInfo.cmake)

file(
    DOWNLOAD
    "https://aka.ms/vs/17/release/vc_redist.x64.exe"
    "${PROJECT_BINARY_DIR}/vc_redist.x64.exe"
)

add_executable(
    ${PROJECT_NAME}
    WIN32
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE src/main.cxx
            src/phosphor.cxx
            data/phosphor.rc
    PRIVATE FILE_SET
            HEADERS
            FILES
            src/phosphor.hxx
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE glow::glow
            glow::flags
)
