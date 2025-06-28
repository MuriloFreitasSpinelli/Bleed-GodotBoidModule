#pragma once
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include "BoidPhysicsRule.h"
#include "../BoidPhysicsStep.h"

using namespace godot;

class BoidPhysicsRuleSet : public Resource {
	GDCLASS(BoidPhysicsRuleSet, Resource)

private:
	String set_name;
	Array incremental_rules; // Array of BoidPhysicsRule references
	Array final_rules;       // Array of BoidPhysicsRule references

	bool rule_matches_criteria(Ref<BoidPhysicsRule> rule, int team, int layer, int sublayer, int range);
	
protected:
	static void _bind_methods();

public:
	//Should be overriden on rule set implementation
	virtual bool should_activate(BoidPhysicsStep step);

	Vector2 apply_incremental_rules_for(int team, int layer, int sublayer, int range, Vector2 current_vel, Vector2 current_pos, Vector2 neighbor_vel, Vector2 neighbor_pos);
	Vector2 apply_final_rules(Vector2 current_vel, Vector2 current_pos, Vector2 neighbor_vel, Vector2 neighbor_pos);

	// Getters and setters for Godot property system
	void set_set_name(const String& p_name) { set_name = p_name; }
	String get_set_name() const { return set_name; }
	
	void set_incremental_rules(const Array& p_rules) { incremental_rules = p_rules; }
	Array get_incremental_rules() const { return incremental_rules; }

	void set_final_rules(const Array& p_rules) { final_rules = p_rules; }
	Array get_final_rules() const { return final_rules; }
};