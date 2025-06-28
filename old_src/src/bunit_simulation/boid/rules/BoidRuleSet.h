#pragma once
#include <godot_cpp/classes/resource.hpp>
#include "../data/BoidSnapshot.h"

using namespace godot;

class BoidRuleSet : public Resource {
	GDCLASS(BoidRuleSet, Resource)

private:
	Array end_rules;
	Array loop_rules;
public:
	Vector2 increment_all_loop_rules_for(BoidSnapshot boid, Vector2 old_vector, int team, int layer);
	Vector2 calculate_all_loop_rules(BoidSnapshot boid, Vector2 current_vector, int n_of_forces);
	Vector2 calculate_all_end_rules(BoidSnapshot boid);
	TypedArray<Vector2> get_empty_increment_vectors();
};