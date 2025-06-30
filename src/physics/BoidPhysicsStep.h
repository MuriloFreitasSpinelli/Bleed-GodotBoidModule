#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/vector2.hpp>

using namespace godot;

class BoidPhysicsStep : public RefCounted {
	GDCLASS(BoidPhysicsStep, RefCounted)

private:
	Array in_range;
	Array in_view;
	Array requested_filters;

	Vector2 new_velocity;

protected:
	static void _bind_methods();

public:


};