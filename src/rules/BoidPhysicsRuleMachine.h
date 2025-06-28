#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include "BoidPhysicsStep.h"
#include "BoidPhysicsRuleSet.h"

using namespace godot;

class BoidPhysicsRuleMachine : public RefCounted {
	GDCLASS(BoidPhysicsRuleMachine, RefCounted)

private:
	Dictionary rule_sets;
	Ref<BoidPhysicsRuleSet> current_rule_set;

protected:
	static void _bind_methods();

public:
	void update_rule_set(BoidPhysicsStep step); // Looks at all the rule sets, sees which ones are valid, the higher to the top in the dictionary the higher the priority
	Ref<BoidPhysicsRuleSet> get_current_rule_set() { return current_rule_set; }

	// Forward methods to current rule set
	Vector2 apply_incremental_rules_for(int team, int layer, int sublayer, Vector2 current_vel, Vector2 current_pos, Vector2 neighbor_vel, Vector2 neighbor_pos);
	Vector2 apply_final_rules(Vector2 current_vel, Vector2 current_pos, Vector2 neighbor_vel, Vector2 neighbor_pos);

	// Getters and setters for Godot property system
	void set_rule_sets(const Dictionary& p_rule_sets) { rule_sets = p_rule_sets; }
	Dictionary get_rule_sets() const { return rule_sets; }
};