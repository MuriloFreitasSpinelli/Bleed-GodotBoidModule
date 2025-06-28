#pragma once
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include "BoidPhysicsEnums.h"

using namespace godot;

class BoidPhysicsRule : public Resource {
	GDCLASS(BoidPhysicsRule, Resource)

private:
	Dictionary ranges;    // range_id -> bool (true means rule applies to this range)
	Dictionary teams;     // team_id -> bool (true means rule applies to this team)
	Dictionary layers;    // layer_id -> bool (true means rule applies to this layer)
	Dictionary sublayers; // sublayer_id -> bool (true means rule applies to this sublayer)

protected:
	static void _bind_methods();

public:
	// Virtual method to be overridden by specific rule implementations
	virtual Vector2 apply_rule(Vector2 current_vel, Vector2 current_position, Vector2 neighbor_vel, Vector2 neighbor_pos);

	// Check if this rule should be applied given the criteria
	bool should_apply(int team, int layer, int sublayer, int range) const;

	// Getters for the filtering criteria
	Dictionary get_ranges() const { return ranges; }
	Dictionary get_teams() const { return teams; }
	Dictionary get_layers() const { return layers; }
	Dictionary get_sublayers() const { return sublayers; }

	// Setters for Godot property system
	void set_ranges(const Dictionary& p_ranges) { ranges = p_ranges; }
	void set_teams(const Dictionary& p_teams) { teams = p_teams; }
	void set_layers(const Dictionary& p_layers) { layers = p_layers; }
	void set_sublayers(const Dictionary& p_sublayers) { sublayers = p_sublayers; }
};