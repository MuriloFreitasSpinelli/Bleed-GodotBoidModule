#pragma once
#include <godot_cpp/classes/ref_counted.hpp>
#include "../physics/BoidPhysicsBody2D.h"
using namespace godot;

struct FilteredNeighbors {
    TypedArray<PackedVector2Array> simd_data;        // For rule calculations
    TypedArray<PackedInt32Array> requested_neighbors; // For unit identification
};

class BoidFilter : public RefCounted {
    GDCLASS(BoidFilter, RefCounted)
private:
    Dictionary rule_filter_requests; // [range][team][layer][sublayer] = index
    Dictionary unit_filter_requests; // [range][team][layer][sublayer] = index

    Array retired_rule_ids;  // Reusable indices from removed rule requests
    Array retired_unit_ids;  // Reusable indices from removed unit requests
    int next_rule_id;        // Counter for rule request indices
    int next_unit_id;        // Counter for unit request indices

    // Helper methods for finding matching requests
    Array get_matching_rule_indices(Ranges range, Teams team, Layers layer, SubLayers sublayer);
    Array get_matching_unit_indices(Ranges range, Teams team, Layers layer, SubLayers sublayer);

protected:
    static void _bind_methods();

public:
    BoidFilter(TypedArray<Array> unit_filter_requests, TypedArray<Array> rule_filter_requests);
    ~BoidFilter();

    FilteredNeighbors filter_neighbors(TypedArray<BoidPhysicsBody2D> neighbors, Vector2 boid_position, Vector2 boid_velocity, float view_angle, float view_range);

    // Methods for managing requests dynamically
    int add_rule_request(Array request);     // Returns index assigned to this request
    bool remove_rule_request(Array request); // Returns true if successfully removed
    int add_unit_request(Array request);     // Returns index assigned to this request
    bool remove_unit_request(Array request); // Returns true if successfully removed
};