#pragma once
#include <godot_cpp/classes/ref_counted.hpp>
#include "NearbyBoids.h"

using namespace godot;

class BoidStep : public RefCounted {
	GDCLASS(BoidStep, RefCounted)

private:
	Vector2 velocity;
	Ref<NearbyBoids> nearby;

protected:
	static void _bind_methods();

public:
	BoidStep();
	BoidStep(Vector2 velocity, Ref<NearbyBoids> nearby);

	Vector2 get_velocity() const { return velocity; }
	Ref<NearbyBoids> get_nearby() const { return nearby; }

	void set_velocity(Vector2 v) { velocity = v}
	void set_nearby(Ref<NearbyBoid> v) { nearby = v; }
};