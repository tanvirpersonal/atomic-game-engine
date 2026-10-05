#pragma once

#include <array>

namespace atomic {

struct Vertex2D {
    float x;
    float y;
};

// CPU-side representation of the first renderable primitive.
// The GPU backend will consume this data in the next lesson.
struct Triangle {
    std::array<Vertex2D, 3> vertices{{
        {-0.6f, -0.5f},
        { 0.6f, -0.5f},
        { 0.0f,  0.6f}
    }};
};

} // namespace atomic
