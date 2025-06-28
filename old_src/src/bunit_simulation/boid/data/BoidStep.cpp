#include "BoidStep.h"

using namespace godot;

BoidStep::BoidStep() : velocity(Vector2()), nearby() {}

BoidStep::BoidStep(Vector2 p_velocity, NearbyBoids p_nearby)
    : velocity(p_velocity), nearby(p_nearby) {
}

void BoidStep::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_velocity"), &BoidStep::get_velocity);
    ClassDB::bind_method(D_METHOD("get_nearby"), &BoidStep::get_nearby);
    ClassDB::bind_method(D_METHOD("set_velocity", "v"), &BoidStep::set_velocity);
    ClassDB::bind_method(D_METHOD("set_nearby", "v"), &BoidStep::set_nearby);
}
