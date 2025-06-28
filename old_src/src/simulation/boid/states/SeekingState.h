#pragma once
#include "BoidBehaviourState.h"

using namespace godot;

class SeekingState : BoidBehaviourState {
	GDCLASS(SeekingState, BoidBehaviourState)

protected:
	static void _bind_methods();

public:
	BoidStep simulate_step(BoidSnapshot boid, Array nearby_boids) override;
};