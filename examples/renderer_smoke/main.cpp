#include <atomic/renderer/triangle.hpp>
#include <iostream>

int main() {
    atomic::Triangle triangle;

    std::cout << "Atomic Renderer Smoke Test\n"
              << "Primitive: triangle\n"
              << "Vertices: " << triangle.vertices.size() << "\n";

    for (const auto& vertex : triangle.vertices) {
        std::cout << "  (" << vertex.x << ", " << vertex.y << ")\n";
    }

    return 0;
}
