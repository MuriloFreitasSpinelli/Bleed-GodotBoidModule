#ifndef BIRDUNIT_H
#define BIRDUNIT_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/vector2i.hpp>
#include "simulation/boid/boid_logic.h"

// Forward declarations
class BoidData;
class BoidLogic;


using namespace godot;

class BirdUnit : public Node2D {
    GDCLASS(BirdUnit, Node2D);

private:
    int unique_id;
    int unit_id;
	int team_id;
    Vector2i grid_position;
    Vector2 velocity;

    BirdUnit boid_target;
    Ref<BoidData> boid_data;
    Ref<BoidLogic> boid_logic;

	Filtered old_nearby_boids;

    bool boid_seeking_active;

protected:
    static void _bind_methods();

public:
    BirdUnit();
    ~BirdUnit();

    // Simulation methods
    void apply_boid_step(const Vector2 &velocity, float delta);

	int get_unique_id() const;
	int get_unit_id() const;
	int get_team_id() const;
	Vector2i get_grid_position() const;
	Vector2 get_boid_velocity() const;
	Vector2 get_boid_target_pos() const;
	Ref<BoidData> get_boid_data() const;
	Ref<BoidLogic> get_boid_logic() const;

	bool get_boid_seeking_active() const;

	void set_unique_id(int value);
	void set_unit_id(int value);
	void set_team_id(int value);
	void set_grid_position(const Vector2i &value);
	void set_boid_velocity(const Vector2 &value);
	void set_boid_target_pos(const Vector2 &value);

	void set_boid_data(Ref<BoidData> value);
	void set_boid_logic(Ref<BoidLogic> value);

	void set_boid_seeking_active(bool value);
};

#endif //BIRDUNIT_H