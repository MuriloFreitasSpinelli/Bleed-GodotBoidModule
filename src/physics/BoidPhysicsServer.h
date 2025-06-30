#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/packed_int32_array.hpp>
#include "BoidPhysicsBody2D.h"
#include "BoidPhysicsStep.h"

using namespace godot;


//Singleton implementation is to use the existant godot autoload
class BoidPhysicsServer : public Node {
	GDCLASS(BoidPhysicsServer, Node)

private:
	int thread_count;
	TypedArray<BoidPhysicsBody2D> registered_boids; // Sorted by unique_id
	TypedArray<PackedInt32Array> boid_chunks;
	PackedInt32Array expired_ids;

	void simulate_boid(BoidPhysicsBody2D* boid);
	void update_boid(BoidPhysicsBody2D* boid, const BoidPhysicsStep& step);

protected:
	//Bind methods with godot engine
	static void _bind_methods();

public:
	BoidPhysicsServer();
	~BoidPhysicsServer();

	// Godot lifecycle methods
	void _ready() override;
	void _physics_process(double delta) override;

	// Boid management
	void register_boid(BoidPhysicsBody2D* boid);
	void unregister_boid(BoidPhysicsBody2D* boid);
	void rebucket_boid(BoidPhysicsBody2D* boid);

	// Getters
	int get_thread_count() const;
	Array get_registered_boids() const;
	PackedInt32Array get_expired_ids() const;
	int get_registered_boid_count() const;
	bool has_boid(BoidPhysicsBody2D* boid) const;

	// Setters
	void set_thread_count(int count);
	void clear_expired_ids();
	void clear_all_boids();
};

