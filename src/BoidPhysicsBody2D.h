#pragma once
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/variant/array.hpp>
#include "BoidPhysicsStep.h"

using namespace godot;

class BoidPhysicsBody2D : public RigidBody2D {
	GDCLASS(BoidPhysicsBody2D, RigidBody2D)

private:
	int unique_id;
	int team_id;
	int layer_id;
	int sublayer_id;
	BoidPhysicsStep old_step;
	Array requested_filters;
	double max_speed;
	double view_angle;
	int view_radius;

protected:
	static void _bind_methods();

public:
	BoidPhysicsBody2D();
	~BoidPhysicsBody2D();

	// Getters
	int get_unique_id() const { return unique_id; }
	int get_team_id() const { return team_id; }
	int get_layer_id() const { return layer_id; }
	int get_sublayer_id() const { return sublayer_id; }
	BoidPhysicsStep get_old_step() const { return old_step; }
	Array get_requested_filters() const { return requested_filters; }
	double get_max_speed() const { return max_speed; }
	double get_view_angle() const { return view_angle; }
	int get_view_radius() const { return view_radius; }

	// Setters
	void set_unique_id(int id) { unique_id = id; }
	void set_team_id(int id) { team_id = id; }
	void set_layer_id(int id) { layer_id = id; }
	void set_sublayer_id(int id) { sublayer_id = id; }
	void set_old_step(const BoidPhysicsStep& step) { old_step = step; }
	void set_requested_filters(const Array& filters) { requested_filters = filters; }
	void set_max_speed(double speed) { max_speed = speed; }
	void set_view_angle(double angle) { view_angle = angle; }
	void set_view_radius(int radius) { view_radius = radius; }

	// Utility methods
	void add_requested_filter(const Variant& filter) { requested_filters.push_back(filter); }
	void remove_requested_filter(const Variant& filter) {
		int index = requested_filters.find(filter);
		if (index != -1) requested_filters.remove_at(index);
	}
	void clear_requested_filters() { requested_filters.clear(); }
	bool has_requested_filter(const Variant& filter) const { return requested_filters.has(filter); }
	int get_requested_filter_count() const { return requested_filters.size(); }
};