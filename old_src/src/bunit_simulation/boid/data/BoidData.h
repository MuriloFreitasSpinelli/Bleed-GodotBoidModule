#pragma once
#include <godot_cpp/classes/resource.hpp>
using namespace godot;

class BoidData : public Resource {
	GDCLASS(BoidData, Resource)

private:
	float max_speed;
	float view_range;
	float view_angle;
	float separation_range;
	float combat_range;

protected:
	static void _bind_methods();

public:

	BoidData();

	// Getters
	float get_max_speed() const { return max_speed; }
	float get_view_range() const { return view_range; }
	float get_view_angle() const { return view_angle; }
	float get_separation_range() const { return separation_range; }
	float get_combat_range() const { return combat_range; }

	// Setters
	void set_max_speed(float value) { max_speed = value; }
	void set_view_range(float value) { view_range = value; }
	void set_view_angle(float value) { view_angle = value; }
	void set_separation_range(float value) { separation_range = value; }
	void set_combat_range(float value) { combat_range = value; }
};