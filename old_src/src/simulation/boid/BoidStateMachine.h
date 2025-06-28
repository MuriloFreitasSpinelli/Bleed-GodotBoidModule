#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/array.hpp>
#include <unordered_map>
//TODO: Include behaviour and snapshot and step

using namespace godot;

enum class StateEnum {
	SEEKING,
	COMBAT,
};

class BoidStateMachine : public Node {
	GDCLASS(BoidStateMachine, Node)

private:
	StateEnum current_state;
	Ref<BoidBehaviourState> seeking_behaviour;
	Ref<BoidBehaviourState> combat_behaviour;
	std::unordered_map<StateEnum, Ref<BoidBehaviourState>> state_map;

protected:
	static void _bind_methods();

public:

	bool change_state(StateEnum new_state);
	BoidStep simulate_state_step(BoidSnapshot boid, Array nearby_boids);

};
