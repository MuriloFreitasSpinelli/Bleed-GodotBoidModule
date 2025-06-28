#include "CombatState.h"
#include <godot_cpp/core/class_db.hpp>

void CombatState::_bind_methods() {

}

BoidStep CombatState::simulate_step(BoidSnapshot boid, Array nearby_boids) override {

    Vector2 current_vel = boid.velocity;
    
    Vector2 forward = current_vel.normalized();
    float half_fov_rad = Math::deg_to_rad(data.view_angle * 0.5f);
    float fov_cos_threshold = Math::cos(half_fov_rad);

    float max_dist_sq = data.view_range * data.view_range;
    float max_c_dist_sq = data.combat_range * data.combat_range;
    float max_speed_sq = data.max_speed * data.max_speed;
    
    Array in_range = [];
    Array in_combat_range = [];
    Array in_view_range = [];
    
    Vector2 avoid_v = Vector2::ZERO;
    
    for (int i = 0; i < nearby_boids.size(); i++) {
        BoidUnit* b = Object::cast_to<BoidUnit>(nearby_boids[i]);
        if (!b) continue;
        
        Vector2 offset = b->get_position() - boid_pos;
        float off_sq = offset.length_squared();
        
        // Early exit for out-of-range boids
        if (off_sq >= max_dist_sq) {
            continue;
        }
        
        filtered.in_range.append(b);
        
        // Optimized avoidance calculation
        float av_range_sq = b->get_separation_range() * b->get_separation_range();
        if (off_sq > 0.0f && off_sq < av_range_sq) {
            float inv_dist = 1.0f / Math::sqrt(off_sq);
            avoid_v += offset * inv_dist * inv_dist;
        }
        
        // Optimized FOV check using dot product
        Vector2 offset_normalized = offset.normalized();
        float dot_product = forward.dot(offset_normalized);
        if (dot_product >= fov_cos_threshold) {
            filtered.in_view_range.append(b);
        }
        
        if (off_sq < max_c_dist_sq) {
            filtered.in_combat_range.append(b);
        }
    }
    
    // Apply forces with proper checks
    int in_range_count = filtered.in_range.size();
    if (in_range_count > 0) {
        Vector2 avoid_force = avoid_v / static_cast<float>(in_range_count);
        float avoid_force_sq = avoid_force.length_squared();
        if (avoid_force_sq > 0.0f) {
            current_vel += avoid_force.normalized() * data.separation_force;
        }
    }
    
    current_vel += world_pull(boid_pos, boid_data);
    
    // Speed limiting using squared comparison
    float speed_sq = current_vel.length_squared();
    if (speed_sq > max_speed_sq) {
        current_vel = current_vel.normalized() * data.max_speed;
    }

    StateEnum next_state;
    if in_combat_range.is_empty() {
        next_state = StateEnum::SEEKING;
    }
    else {
        next_state = StateEnum::COMBAT;
    }
    
    return BoidStep(current_vel, in_range, in_combat_range, in_view_range, next_state);
}
