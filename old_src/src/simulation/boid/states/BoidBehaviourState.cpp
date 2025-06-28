#include "BoidBehaviourState.h"
#include <godot_cpp/core/class_db.hpp>

void BoidBehaviourState::_bind_methods() { }

Vector2 world_pull(BoidSnapshot boid) {
    Vector2 world_v = Vector2(0,0);

    float margin = boid.world_margin;
    float pull = boid.world_pull_force;
    float right_b = boid.right_border;
    float bottom_b = boid.bottom_border;

    float margin_inv = (margin > 0.0f) ? (1.0f / margin) : 0.0f;
    float right_threshold = right_b - margin;
    float bottom_threshold = bottom_b - margin;

    if (boid_pos.x < margin) {
        world_v.x += pull * ((margin - boid_pos.x) * margin_inv);
    }
    else if (boid_pos.x > right_threshold) {
        world_v.x -= pull * ((boid_pos.x - right_threshold) * margin_inv);
    }

    if (boid_pos.y < margin) {
        world_v.y += pull * ((margin - boid_pos.y) * margin_inv);
    }
    else if (boid_pos.y > bottom_threshold) {
        world_v.y -= pull * ((boid_pos.y - bottom_threshold) * margin_inv);
    }

    return world_v;
}