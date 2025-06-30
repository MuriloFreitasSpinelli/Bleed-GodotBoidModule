#include "BoidFilter.h"
#include "../physics/BoidPhysicsBody2D.h"
#include "../BoidPhysicsEnums.h"

void BoidFilter::_bind_methods() {

}

BoidFilter::BoidFilter(TypedArray<Array> unit_filter_requests, TypedArray<Array> rule_filter_requests) {
    next_rule_id = 0;
    next_unit_id = 0;

    // Process rule filter requests
    for (int i = 0; i < rule_filter_requests.size(); i++) {
        Array request = rule_filter_requests[i];
        add_rule_request(request);
    }

    // Process unit filter requests
    for (int i = 0; i < unit_filter_requests.size(); i++) {
        Array request = unit_filter_requests[i];
        add_unit_request(request);
    }
}

BoidFilter::~BoidFilter() {

}

FilteredNeighbors BoidFilter::filter_neighbors(TypedArray<BoidPhysicsBody2D> neighbors, Vector2 boid_position, Vector2 boid_velocity, float view_angle, float view_range) {
    Vector2 boid_vel = boid_velocity;
    Vector2 boid_pos = boid_position;
    Vector2 forward = boid_vel.normalized();
    float half_fov_rad = Math::deg_to_rad(view_angle * 0.5f);
    float fov_cos_threshold = Math::cos(half_fov_rad);
    float max_dist_sq = view_range * view_range;

    // Initialize result arrays based on current request count
    TypedArray<PackedVector2Array> simd_data;
    simd_data.resize(next_rule_id);
    TypedArray<PackedInt32Array> requested_neighbors;
    requested_neighbors.resize(next_unit_id);

    // Range check
    for (int i = 0; i < neighbors.size(); i++) {
        BoidPhysicsBody2D* b = neighbors[i];
        Vector2 offset = b->get_position() - boid_pos;
        float off_sq = offset.length_squared();

        // Early exit for out-of-range boids
        if (off_sq >= max_dist_sq) {
            continue;
        }

        Vector2 offset_normalized = offset.normalized();
        float dot_product = forward.dot(offset_normalized);

        // View angle check
        if (dot_product >= fov_cos_threshold) {
            float distance = Math::sqrt(off_sq);

            for (Ranges range : {Ranges::CLOSE, Ranges::MEDIUM, Ranges::LONG, Ranges::SUPER_LONG}) {
                float range_distance = static_cast<float>(range);
                if (distance <= range_distance) {

                    // Check for rule requests
                    Array rule_indices = get_matching_rule_indices(range, b->get_team_id(), b->get_layer_id(), b->get_sublayer_id());
                    for (int j = 0; j < rule_indices.size(); j++) {
                        int rule_index = rule_indices[j];
                        simd_data[rule_index].append(b->get_position());
                        simd_data[rule_index].append(b->get_velocity());
                    }

                    // Check for unit requests
                    Array unit_indices = get_matching_unit_indices(range, b->get_team_id(), b->get_layer_id(), b->get_sublayer_id());
                    for (int j = 0; j < unit_indices.size(); j++) {
                        int unit_index = unit_indices[j];
                        requested_neighbors[unit_index].append(b->get_instance_id());
                    }

                    break; // Only match the first valid range
                }
            }
        }
    }

    // Return Struct
    FilteredNeighbors result;
    result.simd_data = simd_data;
    result.requested_neighbors = requested_neighbors;
    return result;
}

Array BoidFilter::get_matching_rule_indices(Ranges range, Teams team, Layers layer, SubLayers sublayer) {
    Array matching_indices;

    // Check all possible combinations that match this boid
    Array combinations = {
        Array{range, team, layer, sublayer},      // Exact match
        Array{range, team, layer, SubLayers::ANY}, // Any sublayer
        Array{range, team, Layers::ANY, SubLayers::ANY}, // Any layer/sublayer
        Array{range, Teams::ANY, Layers::ANY, SubLayers::ANY} // Any team/layer/sublayer
    };

    for (int i = 0; i < combinations.size(); i++) {
        Array combo = combinations[i];
        Ranges r = combo[0];
        Teams t = combo[1];
        Layers l = combo[2];
        SubLayers sl = combo[3];

        if (rule_filter_requests.has(r) &&
            rule_filter_requests[r].has(t) &&
            rule_filter_requests[r][t].has(l) &&
            rule_filter_requests[r][t][l].has(sl)) {
            int index = rule_filter_requests[r][t][l][sl];
            matching_indices.append(index);
        }
    }

    return matching_indices;
}

Array BoidFilter::get_matching_unit_indices(Ranges range, Teams team, Layers layer, SubLayers sublayer) {
    Array matching_indices;

    // Check all possible combinations that match this boid
    Array combinations = {
        Array{range, team, layer, sublayer},      // Exact match
        Array{range, team, layer, SubLayers::ANY}, // Any sublayer
        Array{range, team, Layers::ANY, SubLayers::ANY}, // Any layer/sublayer
        Array{range, Teams::ANY, Layers::ANY, SubLayers::ANY} // Any team/layer/sublayer
    };

    for (int i = 0; i < combinations.size(); i++) {
        Array combo = combinations[i];
        Ranges r = combo[0];
        Teams t = combo[1];
        Layers l = combo[2];
        SubLayers sl = combo[3];

        if (unit_filter_requests.has(r) &&
            unit_filter_requests[r].has(t) &&
            unit_filter_requests[r][t].has(l) &&
            unit_filter_requests[r][t][l].has(sl)) {
            int index = unit_filter_requests[r][t][l][sl];
            matching_indices.append(index);
        }
    }

    return matching_indices;
}

int BoidFilter::add_rule_request(Array request) {
    Ranges range = request[0];
    Teams team = request[1];
    Layers layer = request[2];
    SubLayers sublayer = request[3];

    // Check if this exact request already exists
    if (rule_filter_requests.has(range) &&
        rule_filter_requests[range].has(team) &&
        rule_filter_requests[range][team].has(layer) &&
        rule_filter_requests[range][team][layer].has(sublayer)) {
        // Already exists, return existing index
        return rule_filter_requests[range][team][layer][sublayer];
    }

    // Get next available index
    int index;
    if (retired_rule_ids.size() > 0) {
        index = retired_rule_ids[0];
        retired_rule_ids.remove_at(0);
    }
    else {
        index = next_rule_id++;
    }

    // Create nested structure if it doesn't exist
    if (!rule_filter_requests.has(range)) {
        rule_filter_requests[range] = Dictionary();
    }
    if (!rule_filter_requests[range].has(team)) {
        rule_filter_requests[range][team] = Dictionary();
    }
    if (!rule_filter_requests[range][team].has(layer)) {
        rule_filter_requests[range][team][layer] = Dictionary();
    }

    // Add the request
    rule_filter_requests[range][team][layer][sublayer] = index;

    return index;
}

bool BoidFilter::remove_rule_request(Array request) {
    Ranges range = request[0];
    Teams team = request[1];
    Layers layer = request[2];
    SubLayers sublayer = request[3];

    if (rule_filter_requests.has(range) &&
        rule_filter_requests[range].has(team) &&
        rule_filter_requests[range][team].has(layer) &&
        rule_filter_requests[range][team][layer].has(sublayer)) {

        int index = rule_filter_requests[range][team][layer][sublayer];
        rule_filter_requests[range][team][layer].erase(sublayer);
        retired_rule_ids.append(index);
        return true;
    }

    return false;
}

int BoidFilter::add_unit_request(Array request) {
    Ranges range = request[0];
    Teams team = request[1];
    Layers layer = request[2];
    SubLayers sublayer = request[3];

    // Check if this exact request already exists
    if (unit_filter_requests.has(range) &&
        unit_filter_requests[range].has(team) &&
        unit_filter_requests[range][team].has(layer) &&
        unit_filter_requests[range][team][layer].has(sublayer)) {
        // Already exists, return existing index
        return unit_filter_requests[range][team][layer][sublayer];
    }

    // Get next available index
    int index;
    if (retired_unit_ids.size() > 0) {
        index = retired_unit_ids[0];
        retired_unit_ids.remove_at(0);
    }
    else {
        index = next_unit_id++;
    }

    // Create nested structure if it doesn't exist
    if (!unit_filter_requests.has(range)) {
        unit_filter_requests[range] = Dictionary();
    }
    if (!unit_filter_requests[range].has(team)) {
        unit_filter_requests[range][team] = Dictionary();
    }
    if (!unit_filter_requests[range][team].has(layer)) {
        unit_filter_requests[range][team][layer] = Dictionary();
    }

    // Add the request
    unit_filter_requests[range][team][layer][sublayer] = index;

    return index;
}

bool BoidFilter::remove_unit_request(Array request) {
    Ranges range = request[0];
    Teams team = request[1];
    Layers layer = request[2];
    SubLayers sublayer = request[3];

    if (unit_filter_requests.has(range) &&
        unit_filter_requests[range].has(team) &&
        unit_filter_requests[range][team].has(layer) &&
        unit_filter_requests[range][team][layer].has(sublayer)) {

        int index = unit_filter_requests[range][team][layer][sublayer];
        unit_filter_requests[range][team][layer].erase(sublayer);
        retired_unit_ids.append(index);
        return true;
    }

    return false;
}