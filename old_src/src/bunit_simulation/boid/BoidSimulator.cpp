#include "BoidSimulator.h"



BoidSimulator::simulate_boid(Ref<BoidSnapshot> boid, TypedArray<Ref<BoidSnapshot>> nearby) {

	Vector2 velocity = boid->get_velocity();
    Vector2 forward = velocity.normalized();

    float half_fov_rad = Math::deg_to_rad(boid->get_view_angle * 0.5f);
    float fov_cos_threshold = Math::cos(half_fov_rad);

    float view_dist_sq = boid->get_view_range() * boid->get_view_range();
    float combat_dist_sq = boid->get_combat_range() * boid->get_combat_range();

    float max_speed_sq = boid->get_max_speed() * boid->get_max_speed();

    Array in_range = [];
    Array in_combat_range = [];
    Array in_view_range = [];


	for (int i = 0; i < nearby.size(); i++) {
		Ref<BoidSnapshot> other = nearby[i];
        int other_layer = other->get_layer_id();
        int other_team = other->get_team_id();

		Vector2 offset = other->get_position() - boid->get_position();
		float offset_sq = offset.length_squared();

        if (offset_sq >= view_dist_sq) {
            continue;
        }

        //In range
        in_range.append(other);
        //Increment on in range rules

        //Collision
        float av_range_sq = other->get_separation_range() * other->get_separation_range();
        if (offset_sq > 0.0f && offset_sq < av_range_sq) {
            float inv_dist = 1.0f / Math::sqrt(offset_sq);
            avoid_v += offset * inv_dist * inv_dist;
        }

        // Optimized FOV check using dot product
        Vector2 offset_normalized = offset.normalized();
        float dot_product = forward.dot(offset_normalized);
        if (dot_product >= fov_cos_threshold) {
            in_view_range.append(other);
            //increment in_view rules
        }

        if (offset_sq < combat_dist_sq) {
            in_combat_range.append(other);
            //increment in_combat
        }

	}


}