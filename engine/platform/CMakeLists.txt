add_library(atomic_platform STATIC
    src/input.cpp
)

target_include_directories(atomic_platform
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(atomic_platform
    PUBLIC
        atomic_core
)

if(MSVC)
    target_compile_options(atomic_platform PRIVATE /W4 /permissive-)
else()
    target_compile_options(atomic_platform PRIVATE -Wall -Wextra -Wpedantic)
endif()
