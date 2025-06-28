#include "BoidPhysicsRuleMachine.h"
#include <godot_cpp/core/class_db.hpp>

void BoidPhysicsRuleMachine::_bind_methods() {
	ClassDB::bind_method(D_METHOD("update_rule_set", "step"), &BoidPhysicsRuleMachine::update_rule_set);
	ClassDB::bind_method(D_METHOD("get_current_rule_set"), &BoidPhysicsRuleMachine::get_current_rule_set);
	ClassDB::bind_method(D_METHOD("apply_incremental_rules_for", "team", "layer", "sublayer", "current_vel", "current_pos", "neighbor_vel", "neighbor_pos"), &BoidPhysicsRuleMachine::apply_incremental_rules_for);
	ClassDB::bind_method(D_METHOD("apply_final_rules", "current_vel", "current_pos", "neighbor_vel", "neighbor_pos"), &BoidPhysicsRuleMachine::apply_final_rules);

	ClassDB::bind_method(D_METHOD("set_rule_sets", "rule_sets"), &BoidPhysicsRuleMachine::set_rule_sets);
	ClassDB::bind_method(D_METHOD("get_rule_sets"), &BoidPhysicsRuleMachine::get_rule_sets);

	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "rule_sets"), "set_rule_sets", "get_rule_sets");
}

void BoidPhysicsRuleMachine::update_rule_set(BoidPhysicsStep step) {
	// Iterate through rule_sets dictionary in order (higher priority first)
	Array keys = rule_sets.keys();
	for (int i = 0; i < keys.size(); i++) {
		Ref<BoidPhysicsRuleSet> rule_set = rule_sets[keys[i]];
		if (rule_set.is_valid() && rule_set->should_activate(step)) {
			current_rule_set = rule_set;
			return; // Use the first rule set that should activate (highest priority)
		}
	}
	// If no rule set should activate, current_rule_set remains unchanged or null
}

Vector2 BoidPhysicsRuleMachine::apply_incremental_rules_for(int range, int team, int layer, int sublayer, Vector2 current_vel, Vector2 current_pos, Vector2 neighbor_vel, Vector2 neighbor_pos) {
	if (current_rule_set.is_valid()) {

		return current_rule_set->apply_incremental_rules_for(team, layer, sublayer, range, current_vel, current_pos, neighbor_vel, neighbor_pos);
	}
	return Vector2(0, 0); // No rule set active
}

Vector2 BoidPhysicsRuleMachine::apply_final_rules(Vector2 current_vel, Vector2 current_pos, Vector2 neighbor_vel, Vector2 neighbor_pos) {
	if (current_rule_set.is_valid()) {
		return current_rule_set->apply_final_rules(current_vel, current_pos, neighbor_vel, neighbor_pos);
	}
	return current_vel; // Return unchanged velocity if no rule set
}