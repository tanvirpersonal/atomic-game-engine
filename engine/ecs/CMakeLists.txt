add_library(atomic_ecs STATIC
    src/entity.cpp
)

target_include_directories(atomic_ecs
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(atomic_ecs
    PUBLIC
        atomic_core
)

if(MSVC)
    target_compile_options(atomic_ecs PRIVATE /W4 /permissive-)
else()
    target_compile_options(atomic_ecs PRIVATE -Wall -Wextra -Wpedantic)
endif()
