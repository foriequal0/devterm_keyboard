find_package(GTest REQUIRED)

add_executable(lib_test
        debouncer_test.cpp
        joystickstack_test.cpp
        key_test.cpp
        math_test.cpp rollover_test.cpp)

target_link_libraries(lib_test
        PRIVATE gtest_main
        PRIVATE corelib)

include(GoogleTest)
gtest_discover_tests(lib_test)
