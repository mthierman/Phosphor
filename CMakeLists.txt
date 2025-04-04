cmake_minimum_required(VERSION 3.30)
project(Phosphor VERSION 0.0.1)
list(
    APPEND
    CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    )
add_subdirectory(phosphor)
