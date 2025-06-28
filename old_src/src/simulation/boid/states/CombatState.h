#pragma once
#include "BoidBehaviourState.h"

using namespace godot;

class CombatState : BoidBehaviourState {
	GDCLASS(CombatState, BoidBehaviourState)

protected:
	static void _bind_methods();

public:
	BoidStep simulate_step(BoidSnapshot boid, Array nearby_boids) override;
};