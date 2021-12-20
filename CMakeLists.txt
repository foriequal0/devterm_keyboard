cmake_minimum_required(VERSION 3.18)

enable_testing()

project(devterm_keyboard)

set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(src)
add_subdirectory(tests)