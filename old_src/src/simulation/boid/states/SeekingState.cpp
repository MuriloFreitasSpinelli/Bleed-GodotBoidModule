#include "SeekingState.h"
#include <godot_cpp/core/class_db.hpp>

void SeekingState::_bind_methods() {

}

BoidStep SeekingState::simulate_step(BoidSnapshot boid, Array nearby_boids) override {
    Vector2 boid_vel = boid.velocity;

    Vector2 forward = boid_vel.normalized();
    float half_fov_rad = Math::deg_to_rad(boid.view_angle * 0.5f);
    float fov_cos_threshold = Math::cos(half_fov_rad);

    float max_dist_sq = boid.view_range * boid.view_range;
    float max_c_dist_sq = boid.combat_range * boid.combat_range;
    float max_speed_sq = boid.max_speed * boid.max_speed;

    Array in_range = [];
    Array in_combat_range = [];
    Array in_view_range = [];

    Vector2 avoid_v = Vector2(0,0);
    Vector2 cohesion_v = Vector2(0,0);
    Vector2 alignment_v = Vector2(0,0);

    for (int i = 0; i < nearby_boids.size(); i++) {
        BoidUnit* b = nearby_boids[i];

        Vector2 offset = b->get_position() - boid_pos;
        float off_sq = offset.length_squared();

        // Early exit for out-of-range boids
        if (off_sq >= max_dist_sq) {
            continue;
        }

        in_range.append(b)

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
            cohesion_v += b->get_position();
            alignment_v += b->get_velocity();
        }

        if (off_sq < max_c_dist_sq) {
            filtered.in_combat_range.append(b);
        }
    }

    // Apply forces with proper checks
    int in_range_count = in_range.size();
    if (in_range_count > 0) {
        Vector2 avoid_force = avoid_v / static_cast<float>(in_range_count);
        float avoid_force_sq = avoid_force.length_squared();
        if (avoid_force_sq > 0.0f) {
            boid_vel += avoid_force.normalized() * boid.separation_force;
        }
    }

    int view_count = in_view_range.size();
    if (view_count > 0) {
        Vector2 cohesion_target = cohesion_v / static_cast<float>(view_count);
        Vector2 cohesion_force = cohesion_target - boid_pos;
        float cohesion_force_sq = cohesion_force.length_squared();
        if (cohesion_force_sq > 0.0f) {
            boid_vel += cohesion_force.normalized() * boid.cohesion_force;
        }

        Vector2 align_force = alignment_v / static_cast<float>(view_count);
        float align_force_sq = align_force.length_squared();
        if (align_force_sq > 0.0f) {
            boid_vel += align_force.normalized() * boid.alignment_force;
        }
    }

    boid_vel += world_pull(boid)

    // Optimized chase force
    if (chase_pos != Vector2::ZERO) {
        Vector2 chase_dir = chase_pos - boid_pos;
        float chase_dir_sq = chase_dir.length_squared();
        if (chase_dir_sq > 0.0f) {
            boid_vel += chase_dir.normalized() * boid.chase_force;
        }
    }

    // Speed limiting using squared comparison
    float speed_sq = boid_vel.length_squared();
    if (speed_sq > max_speed_sq) {
        boid_vel = boid_vel.normalized() * boid.max_speed;
    }

    StateEnum next_state; 
    if in_combat_range.is_empty() {
        next_state = StateEnum::SEEKING
    }
    else {
        next_state = StateEnum::COMBAT
    }

    return BoidStep(boid_vel, in_range, in_combat_range, in_view_range, next_state);
}