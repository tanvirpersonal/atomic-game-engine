cmake_minimum_required(VERSION 3.20)

project(AtomicGameEngine
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ATOMIC_BUILD_EXAMPLES "Build engine examples" ON)

add_library(atomic_core STATIC
    engine/core/src/application.cpp
    engine/core/src/log.cpp
    engine/core/src/time.cpp
)

target_include_directories(atomic_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/engine/core/include>
)

if(MSVC)
    target_compile_options(atomic_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(atomic_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(ATOMIC_BUILD_EXAMPLES)
    add_executable(atomic_core_smoke examples/core_smoke/main.cpp)
    target_link_libraries(atomic_core_smoke PRIVATE atomic_core)
endif()
