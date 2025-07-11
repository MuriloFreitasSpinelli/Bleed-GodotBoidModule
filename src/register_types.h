#pragma once

#include <godot_cpp/core/class_db.hpp>
#include "boids/boid.h"
#include "boids/boids_handler.h"

void initialize_modules(ModuleInitializationLevel p_level);
void uninitialize_modules(ModuleInitializationLevel p_level);
