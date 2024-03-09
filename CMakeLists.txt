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

add_executable(
    ${PROJECT_NAME}
    WIN32
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE src/main.cxx
            data/phosphor.rc
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE glow::glow
            glow::flags
)
