#pragma once
#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

class BoidSnapshot : public RefCounted {
	GDCLASS(BoidSnapshot, RefCounted)

private:
	int unique_id;
	int layer_id;
	int team_id;
	Vector2 position;
	Vector2 velocity;

	float view_angle;

	float view_range;
	float combat_range;
	float separation_range;

	float max_speed;

protected:
	static void _bind_methods();

public:
	//Creation
	BoidSnapshot();
	BoidSnapshot(int uid, int lid, int tid, Vector2 pos, Vector2 vel,
		float v_angle, float v_range, float c_range, float s_range, float m_speed);

	//Getters
	int get_unique_id() const { return unique_id; }
	int get_layer_id() const { return layer_id; }
	int get_team_id() const { return team_id; }
	Vector2 get_position() const { return position; }
	Vector2 get_velocity() const { return velocity; }
	float get_view_angle() const { return view_angle; }
	float get_view_range() const { return view_range; }
	float get_combat_range() const { return combat_range; }
	float get_separation_range() const { return separation_range; }
	float get_max_speed() const { return max_speed; }

	//Setters
	void set_unique_id(int value) { unique_id = value; }
	void set_layer_id(int value) { layer_id = value; }
	void set_team_id(int value) { team_id = value; }
	void set_position(Vector2 value) { position = value; }
	void set_velocity(Vector2 value) { velocity = value; }
	void set_view_angle(float value) { view_angle = value; }
	void set_view_range(float value) { view_range = value; }
	void set_combat_range(float value) { combat_range = value; }
	void set_separation_range(float value) { separation_range = value; }
	void set_max_speed(float value) { max_speed = value; }
};