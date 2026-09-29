#pragma once

#include <cstdint>
#include <limits>
#include <vector>

namespace atomic::ecs {

struct Entity {
    using Index = std::uint32_t;
    using Generation = std::uint32_t;

    static constexpr Index InvalidIndex = std::numeric_limits<Index>::max();

    Index index = InvalidIndex;
    Generation generation = 0;

    constexpr bool valid() const noexcept {
        return index != InvalidIndex;
    }

    friend constexpr bool operator==(Entity a, Entity b) noexcept {
        return a.index == b.index && a.generation == b.generation;
    }

    friend constexpr bool operator!=(Entity a, Entity b) noexcept {
        return !(a == b);
    }
};

class EntityManager {
public:
    explicit EntityManager(std::uint32_t initial_capacity = 1024);

    Entity create();
    bool destroy(Entity entity) noexcept;
    bool alive(Entity entity) const noexcept;

    std::uint32_t alive_count() const noexcept { return alive_count_; }
    std::uint32_t capacity() const noexcept {
        return static_cast<std::uint32_t>(generations_.size());
    }

private:
    struct Slot {
        Entity::Generation generation = 1;
        bool alive = false;
    };

    std::vector<Slot> slots_;
    std::vector<Entity::Index> free_indices_;
    std::uint32_t alive_count_ = 0;
};

} // namespace atomic::ecs
