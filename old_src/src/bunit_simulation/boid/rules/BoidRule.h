#pragma once
#include <godot_cpp/classes/resource.hpp>
#include "../data/BoidSnapshot.h"
#include "../../GlobalVariables.h"
using namespace godot;


enum RangeFlags {
	RANGE_IN_VIEW = 1 << 0,
	RANGE_IN_COMBAT = 1 << 1,
};

enum LoopPosition {
	IN_LOOP,
	OUTSIDE_LOOP,
};

//Add trait enum and other things for unit here
class BoidRule : public Resource {
	GDCLASS(BoidRule, Resource)
private:
	LoopPosition loop_position;
	int range_flags;    // Can combine multiple RangeFlags
	int layer_flags;    // Can combine multiple LayerFlags  
	int team_flags;     // Can combine multiple TeamFlags

protected:
	static void bind_methods()

public:
	virtual Vector2 caculate_vector(BoidSnapshot boid) { return Vector2(); }
	virtual Vector2 increment_vector(BoidSnapshot boid, Vector2 old_vector) { return Vector2(); }
	virtual Vector2 calculate_increment_vector(BoidSnapshot boid, Vector2 final_vector, int n_of_forces) { return Vector2(); }

	LoopPosition get_loop_position() { return loop_position; }
	void set_loop_position(LoopPosition v) { loop_position = v; }

	// Range flags
	int  get_range_flags() const { return range_flags; }
	void set_range_flags(int flags) { range_flags = flags; }
	bool has_range_flag(RangeFlags flag) const { return (range_flags & flag) != 0; }
	void add_range_flag(RangeFlags flag) { range_flags |= flag; }
	void remove_range_flag(RangeFlags flag) { range_flags &= ~flag; }

	// Layer flags
	int  get_layer_flags() const { return layer_flags; }
	void set_layer_flags(int flags) { layer_flags = flags; }
	bool has_layer_flag(LayerFlags flag) const { return (layer_flags & flag) != 0; }
	void add_layer_flag(LayerFlags flag) { layer_flags |= flag; }
	void remove_layer_flag(LayerFlags flag) { layer_flags &= ~flag; }

	// Team flags
	int  get_team_flags() const { return team_flags; }
	void set_team_flags(int flags) { team_flags = flags; }
	bool has_team_flag(TeamFlags flag) const { return (team_flags & flag) != 0; }
	void add_team_flag(TeamFlags flag) { team_flags |= flag; }
	void remove_team_flag(TeamFlags flag) { team_flags &= ~flag; }
};