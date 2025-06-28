#pragma once
#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

class BoidStep : public RefCounted {
	GDCLASS(BoidStep, RefCounted)
		
public:
	Vector2 velociy;
	Array in_range;
	Array in_combat_range;
	Arrau in_view_range;
	StateEnum next_state;

protected:
	static void _bind_methods();
};