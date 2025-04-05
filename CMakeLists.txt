cmake_minimum_required(VERSION 3.31)
project(phosphor)
list(
    APPEND
    CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    )
add_subdirectory(phosphor)
