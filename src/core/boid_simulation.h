#pragma once
#include <godot_cpp/classes/node.hpp>
#include "../compute_shaders/boid_compute.h"
#include "../boids/boids_handler.h"

using namespace godot;

class BoidSimulation : public Node {
	GDCLASS(BoidSimulation, Node)

private:
	BoidCompute* boid_compute;
	BoidsHandler* boid_handler;
protected:

public:

};