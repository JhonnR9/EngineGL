//
// Created by jhone on 26/03/2026.
//

#ifndef COLLISION_DETECTION_SYSTEM_H
#define COLLISION_DETECTION_SYSTEM_H

#include "system.h"
#include <unordered_map>
#include <vector>
#include <utility>
#include "entt/entt.hpp"
#include "components/components.h"

class ShapeRenderer;

class CollisionDetectionSystem final : public System {
    struct PairHash {
        std::size_t operator()(const std::pair<int, int> &p) const {
            return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
        }
    };

    struct EntityPairHash {
        std::size_t operator()(const std::pair<entt::entity, entt::entity> &pair) const;
    };

    // Equality check for entity pairs, order-independent.
    struct EntityPairEqual {
        bool operator()(
            const std::pair<entt::entity, entt::entity> &lhs,
            const std::pair<entt::entity, entt::entity> &rhs
        ) const;
    };

    // Local results for each thread
    struct CollisionResult {
        std::unordered_set<std::pair<entt::entity, entt::entity>, EntityPairHash, EntityPairEqual> pairs;
    };

    float hash_grid_cell_size{150.0f};
    std::unordered_map<std::pair<int, int>, std::vector<entt::entity>, PairHash> hash_grid_cells;

    std::pair<int, int> get_hash_grid_cell(float x, float y) const;

    void register_entity_in_grid(entt::entity entity, const Vector2 &position);

    std::vector<entt::entity> &get_nearby_entities(const Vector2 &position);

    void populate_hash_grid_cells();

    void check_collision(const std::vector<entt::entity> &entities, CollisionResult &local_collision_result);

    ShapeRenderer* shape_renderer{nullptr};
    void draw_collider_debug(entt::entity entity);
public:
    explicit CollisionDetectionSystem(entt::registry &registry);

    void run(float dt) override;
};



#endif //COLLISION_DETECTION_SYSTEM_H
