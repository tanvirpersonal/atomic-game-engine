add_library(atomic_renderer INTERFACE)

target_include_directories(atomic_renderer
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(atomic_renderer
    INTERFACE
        atomic_core
)
