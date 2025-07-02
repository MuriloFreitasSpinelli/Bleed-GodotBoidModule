#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class BoidStateRule : public Resource {
	GDCLASS(BoidStateRule, Resource)

private:
	uint64_t key_request;

protected:

public:
	Vector2 solve(std::vector<Vector2> positions, std::vector<Vector2> velocities, Vector2 position, Vector2 velocity);

};
