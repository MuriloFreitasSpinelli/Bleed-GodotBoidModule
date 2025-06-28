#pragma once
#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class BoidBehaviourState : public Resource {
	GDCLASS(BoidBehaviourState, Resource)

protected:
	static void _bind_methods();

public:
	virtual BoidStep simulate_step(BoidSnapshot boid, Array nearby_boids) = 0;
	Vector2 world_pull(BoidSnapshot boid);
};