#include "atomic/ecs/entity.hpp"

#include <stdexcept>

namespace atomic::ecs {

EntityManager::EntityManager(std::uint32_t initial_capacity) {
    slots_.reserve(initial_capacity);
    free_indices_.reserve(initial_capacity);
}

Entity EntityManager::create() {
    Entity::Index index;

    if (!free_indices_.empty()) {
        index = free_indices_.back();
        free_indices_.pop_back();

        Slot& slot = slots_[index];
        slot.alive = true;
        ++alive_count_;
        return Entity{index, slot.generation};
    }

    if (slots_.size() >= Entity::InvalidIndex) {
        throw std::overflow_error("Atomic ECS entity index space exhausted");
    }

    index = static_cast<Entity::Index>(slots_.size());
    slots_.push_back(Slot{1, true});
    ++alive_count_;
    return Entity{index, 1};
}

bool EntityManager::destroy(Entity entity) noexcept {
    if (!alive(entity)) return false;

    Slot& slot = slots_[entity.index];
    slot.alive = false;
    ++slot.generation;

    if (slot.generation == 0) {
        slot.generation = 1;
    }

    free_indices_.push_back(entity.index);
    --alive_count_;
    return true;
}

bool EntityManager::alive(Entity entity) const noexcept {
    return entity.valid()
        && entity.index < slots_.size()
        && slots_[entity.index].alive
        && slots_[entity.index].generation == entity.generation;
}

} // namespace atomic::ecs
