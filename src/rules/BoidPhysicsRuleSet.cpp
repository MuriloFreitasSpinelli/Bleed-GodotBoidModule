#include "BoidPhysicsRuleSet.h"
#include <godot_cpp/core/class_db.hpp>

void BoidPhysicsRuleSet::_bind_methods() {
	ClassDB::bind_method(D_METHOD("should_activate", "step"), &BoidPhysicsRuleSet::should_activate);
	ClassDB::bind_method(D_METHOD("apply_incremental_rules_for", "team", "layer", "sublayer", "range", "current_vel", "current_pos", "neighbor_vel", "neighbor_pos"), &BoidPhysicsRuleSet::apply_incremental_rules_for);
	ClassDB::bind_method(D_METHOD("apply_final_rules", "current_vel", "current_pos", "neighbor_vel", "neighbor_pos"), &BoidPhysicsRuleSet::apply_final_rules);

	ClassDB::bind_method(D_METHOD("set_set_name", "name"), &BoidPhysicsRuleSet::set_set_name);
	ClassDB::bind_method(D_METHOD("get_set_name"), &BoidPhysicsRuleSet::get_set_name);
	ClassDB::bind_method(D_METHOD("set_incremental_rules", "rules"), &BoidPhysicsRuleSet::set_incremental_rules);
	ClassDB::bind_method(D_METHOD("get_incremental_rules"), &BoidPhysicsRuleSet::get_incremental_rules);
	ClassDB::bind_method(D_METHOD("set_final_rules", "rules"), &BoidPhysicsRuleSet::set_final_rules);
	ClassDB::bind_method(D_METHOD("get_final_rules"), &BoidPhysicsRuleSet::get_final_rules);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "set_name"), "set_set_name", "get_set_name");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "incremental_rules"), "set_incremental_rules", "get_incremental_rules");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "final_rules"), "set_final_rules", "get_final_rules");
}

bool BoidPhysicsRuleSet::should_activate(BoidPhysicsStep step) {
	// Implement your activation condition logic here
	// This could check step conditions like:
	// - Current simulation time
	// - Environmental conditions  
	// - Boid state (health, energy, etc.)
	// - Trigger events
	// - Phase of behavior (hunting, flocking, escaping, etc.)

	// Example conditions might be:
	// if (step.simulation_time > activation_time && step.simulation_time < deactivation_time)
	// if (step.threat_level > threat_threshold)
	// if (step.food_availability < hunger_threshold)

	return true; // Placeholder - implement your activation logic
}

bool BoidPhysicsRuleSet::rule_matches_criteria(Ref<BoidPhysicsRule> rule, int team, int layer, int sublayer, int range) {
	if (!rule.is_valid()) {
		return false;
	}

	return rule->should_apply(team, layer, sublayer, range);
}

Vector2 BoidPhysicsRuleSet::apply_incremental_rules_for(int range, int team, int layer, int sublayer, int range, Vector2 current_vel, Vector2 current_pos, Vector2 neighbor_vel, Vector2 neighbor_pos) {
	Vector2 accumulated_force = Vector2(0, 0);

	// Apply all matching incremental rules
	for (int i = 0; i < incremental_rules.size(); i++) {
		Ref<BoidPhysicsRule> rule = incremental_rules[i];
		if (rule_matches_criteria(rule, range, team, layer, sublayer, range)) {
			Vector2 rule_force = rule->apply_rule(current_vel, current_pos, neighbor_vel, neighbor_pos);
			accumulated_force += rule_force;
		}
	}

	return accumulated_force;
}

Vector2 BoidPhysicsRuleSet::apply_final_rules(Vector2 current_vel, Vector2 current_pos, Vector2 neighbor_vel, Vector2 neighbor_pos) {
	Vector2 final_velocity = current_vel;

	// Apply all final rules (these might modify velocity directly rather than adding forces)
	for (int i = 0; i < final_rules.size(); i++) {
		Ref<BoidPhysicsRule> rule = final_rules[i];
		// Final rules typically don't need criteria filtering since they're applied to all
		Vector2 rule_result = rule->apply_rule(final_velocity, current_pos, neighbor_vel, neighbor_pos);
		final_velocity = rule_result; // Final rules replace rather than accumulate
	}

	return final_velocity;
}