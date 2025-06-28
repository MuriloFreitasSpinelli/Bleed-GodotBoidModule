#pragma once
#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

class NearbyBoids : public RefCounted {
	GDCLASS(NearbyBoids, RefCounted)

private:
	Array in_range; //team[layer[boids[boid]]
	Array in_view;
	Array in_combat;

protected:
	static void _bind_methods();

public:
	//Creation
	NearbyBoids();
	NearbyBoids(Array in_range, Array in_view, Array in_combat);

	//Getters
	Array get_in_range() const { return in_range; }
	Array get_in_view() const { return in_view; }
	Array get_in_combat() const { return in_combat; }
};