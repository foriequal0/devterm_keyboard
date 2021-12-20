add_library(corelib
        debouncer.hpp
        glider.cpp
        glider.hpp
        joystickstack.cpp
        joystickstack.cpp
        key.cpp
        key.hpp
        keyset.hpp
        math.cpp
        math.hpp
        ratemeter.cpp
        ratemeter.hpp
        rollover.hpp
        )

target_include_directories(corelib PUBLIC ${CMAKE_CURRENT_LIST_DIR})