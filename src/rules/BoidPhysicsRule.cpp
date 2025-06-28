#include "BoidPhysicsRule.h"
#include <godot_cpp/core/class_db.hpp>

void BoidPhysicsRule::_bind_methods() {
	ClassDB::bind_method(D_METHOD("apply_rule", "current_vel", "current_position", "neighbor_vel", "neighbor_pos"), &BoidPhysicsRule::apply_rule);
	ClassDB::bind_method(D_METHOD("should_apply", "team", "layer", "sublayer", "range"), &BoidPhysicsRule::should_apply);

	ClassDB::bind_method(D_METHOD("set_ranges", "ranges"), &BoidPhysicsRule::set_ranges);
	ClassDB::bind_method(D_METHOD("get_ranges"), &BoidPhysicsRule::get_ranges);
	ClassDB::bind_method(D_METHOD("set_teams", "teams"), &BoidPhysicsRule::set_teams);
	ClassDB::bind_method(D_METHOD("get_teams"), &BoidPhysicsRule::get_teams);
	ClassDB::bind_method(D_METHOD("set_layers", "layers"), &BoidPhysicsRule::set_layers);
	ClassDB::bind_method(D_METHOD("get_layers"), &BoidPhysicsRule::get_layers);
	ClassDB::bind_method(D_METHOD("set_sublayers", "sublayers"), &BoidPhysicsRule::set_sublayers);
	ClassDB::bind_method(D_METHOD("get_sublayers"), &BoidPhysicsRule::get_sublayers);

	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "ranges"), "set_ranges", "get_ranges");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "teams"), "set_teams", "get_teams");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "layers"), "set_layers", "get_layers");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "sublayers"), "set_sublayers", "get_sublayers");
}

Vector2 BoidPhysicsRule::apply_rule(Vector2 current_vel, Vector2 current_position, Vector2 neighbor_vel, Vector2 neighbor_pos) {
	// Base implementation - should be overridden by specific rule types
	// This is the abstract method that concrete rules will implement
	return Vector2(0, 0);
}

bool BoidPhysicsRule::should_apply(int team, int layer, int sublayer, int range) const {
	// Check if this rule applies to the given criteria
	// Dictionary values of 'true' mean the rule applies, 'false' or missing means it doesn't


	if (ranges.has(range)) {
		bool range_applies = ranges[range];
		if (!range_applies) return false;
	}

	// Check team criteria
	if (teams.has(team)) {
		bool team_applies = teams[team];
		if (!team_applies) return false;
	}

	// Check layer criteria
	if (layers.has(layer)) {
		bool layer_applies = layers[layer];
		if (!layer_applies) return false;
	}

	// Check sublayer criteria
	if (sublayers.has(sublayer)) {
		bool sublayer_applies = sublayers[sublayer];
		if (!sublayer_applies) return false;
	}

	// Check range criteria
	if (ranges.has(range)) {
		bool range_applies = ranges[range];
		if (!range_applies) return false;
	}

	// If we get here, all criteria that were specified are met
	return true;
}