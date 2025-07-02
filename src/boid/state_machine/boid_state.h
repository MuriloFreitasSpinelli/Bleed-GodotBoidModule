#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/resource.hpp>

using namespace godot;

//TODO: Add godot bindings after all logic is finished
class BoidState : public Resource {
	GDCLASS(BoidState, Resource)

private:
	std::vector<BoidStateRule> rules;
	std::vector<uint64_t> key_requests;

protected:
	static void _bind_methods();

public:
	Vector2 solve_next_velocity(NeighborMap& nb_map, Vector2 position, Vector2 velocity, float max_speed)

};
