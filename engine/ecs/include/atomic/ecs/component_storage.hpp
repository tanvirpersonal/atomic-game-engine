#pragma once

#include "atomic/ecs/entity.hpp"

#include <cassert>
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

namespace atomic::ecs {

template <typename T>
class ComponentStorage {
public:
    explicit ComponentStorage(std::size_t entity_capacity = 1024)
        : sparse_(entity_capacity, kInvalid) {}

    ComponentStorage(const ComponentStorage&) = delete;
    ComponentStorage& operator=(const ComponentStorage&) = delete;

    bool contains(Entity entity) const noexcept {
        return entity.valid()
            && entity.index < sparse_.size()
            && sparse_[entity.index] != kInvalid
            && sparse_[entity.index] < dense_entities_.size()
            && dense_entities_[sparse_[entity.index]] == entity;
    }

    template <typename... Args>
    T& emplace(Entity entity, Args&&... args) {
        assert(entity.valid());
        ensure_capacity(entity.index);

        if (contains(entity)) {
            return dense_components_[sparse_[entity.index]];
        }

        const std::size_t dense_index = dense_entities_.size();
        dense_entities_.push_back(entity);
        sparse_[entity.index] = dense_index;

        try {
            dense_components_.emplace_back(std::forward<Args>(args)...);
        } catch (...) {
            sparse_[entity.index] = kInvalid;
            dense_entities_.pop_back();
            throw;
        }

        return dense_components_.back();
    }

    bool remove(Entity entity) noexcept {
        if (!contains(entity)) return false;

        const std::size_t removed = sparse_[entity.index];
        const std::size_t last = dense_entities_.size() - 1;

        if (removed != last) {
            dense_entities_[removed] = dense_entities_[last];
            dense_components_[removed] = std::move(dense_components_[last]);
            sparse_[dense_entities_[removed].index] = removed;
        }

        dense_entities_.pop_back();
        dense_components_.pop_back();
        sparse_[entity.index] = kInvalid;
        return true;
    }

    T* get(Entity entity) noexcept {
        return contains(entity) ? &dense_components_[sparse_[entity.index]] : nullptr;
    }

    const T* get(Entity entity) const noexcept {
        return contains(entity) ? &dense_components_[sparse_[entity.index]] : nullptr;
    }

    std::size_t size() const noexcept { return dense_entities_.size(); }
    bool empty() const noexcept { return dense_entities_.empty(); }

    const std::vector<Entity>& entities() const noexcept {
        return dense_entities_;
    }

    std::vector<T>& data() noexcept { return dense_components_; }
    const std::vector<T>& data() const noexcept { return dense_components_; }

private:
    static constexpr std::size_t kInvalid = static_cast<std::size_t>(-1);

    void ensure_capacity(Entity::Index index) {
        if (index >= sparse_.size()) {
            sparse_.resize(static_cast<std::size_t>(index) + 1, kInvalid);
        }
    }

    std::vector<std::size_t> sparse_;
    std::vector<Entity> dense_entities_;
    std::vector<T> dense_components_;
};

} // namespace atomic::ecs
