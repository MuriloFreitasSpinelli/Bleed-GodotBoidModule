#include "birdunit.h"
#include "simulation/boid/boid_data.h"
#include "simulation/boid/boid_logic.h"

using namespace godot;

BirdUnit::BirdUnit() {
    unique_id = 0;
    unit_id = 0;
    team_id = 0;
    grid_position = Vector2i();
    velocity = Vector2();
    boid_seeking_active = false;
}

BirdUnit::~BirdUnit() = default;

void BirdUnit::_bind_methods() {
    // Getters
    ClassDB::bind_method(D_METHOD("get_unique_id"), &BirdUnit::get_unique_id);
    ClassDB::bind_method(D_METHOD("get_unit_id"), &BirdUnit::get_unit_id);
    ClassDB::bind_method(D_METHOD("get_team_id"), &BirdUnit::get_team_id);
    ClassDB::bind_method(D_METHOD("get_grid_position"), &BirdUnit::get_grid_position);
    ClassDB::bind_method(D_METHOD("get_boid_velocity"), &BirdUnit::get_boid_velocity);
    ClassDB::bind_method(D_METHOD("get_boid_target_pos"), &BirdUnit::get_boid_target_pos);
    ClassDB::bind_method(D_METHOD("get_boid_data"), &BirdUnit::get_boid_data);
    ClassDB::bind_method(D_METHOD("get_boid_logic"), &BirdUnit::get_boid_logic);
    ClassDB::bind_method(D_METHOD("get_boid_seeking_active"), &BirdUnit::get_boid_seeking_active);

    // Setters
    ClassDB::bind_method(D_METHOD("set_unique_id", "value"), &BirdUnit::set_unique_id);
    ClassDB::bind_method(D_METHOD("set_unit_id", "value"), &BirdUnit::set_unit_id);
    ClassDB::bind_method(D_METHOD("set_team_id", "value"), &BirdUnit::set_team_id);
    ClassDB::bind_method(D_METHOD("set_grid_position", "value"), &BirdUnit::set_grid_position);
    ClassDB::bind_method(D_METHOD("set_boid_velocity", "value"), &BirdUnit::set_boid_velocity);
    ClassDB::bind_method(D_METHOD("set_boid_target_pos", "value"), &BirdUnit::set_boid_target_pos);
    ClassDB::bind_method(D_METHOD("set_boid_data", "value"), &BirdUnit::set_boid_data);
    ClassDB::bind_method(D_METHOD("set_boid_logic", "value"), &BirdUnit::set_boid_logic);
    ClassDB::bind_method(D_METHOD("set_boid_seeking_active", "value"), &BirdUnit::set_boid_seeking_active);

    // Simulation methods
    ClassDB::bind_method(D_METHOD("simulate_boid_step", "nearby_boids"), &BirdUnit::simulate_boid_step);
    ClassDB::bind_method(D_METHOD("apply_boid_step", "velocity", "delta"), &BirdUnit::apply_boid_step);
}

void BirdUnit::apply_boid_step(const BoidStep& boid_step, float delta) {
    velocity = boid_step.next_vel;
    set_position(get_position() + velocity * delta);
}

// Getters
int BirdUnit::get_unique_id() const {
    return unique_id;
}

int BirdUnit::get_unit_id() const {
    return unit_id;
}

int BirdUnit::get_team_id() const {
    return team_id;
}

Vector2i BirdUnit::get_grid_position() const {
    return grid_position;
}

Vector2 BirdUnit::get_boid_velocity() const {
    return velocity;
}

Vector2 BirdUnit::get_boid_target_pos() const {
    return boid_target.get_position();
}

Ref<BoidData> BirdUnit::get_boid_data() const {
    return boid_data;
}

Ref<BoidLogic> BirdUnit::get_boid_logic() const {
    return boid_logic;
}

bool BirdUnit::get_boid_seeking_active() const {
    return boid_seeking_active;
}

// Setters
void BirdUnit::set_unique_id(int value) {
    unique_id = value;
}

void BirdUnit::set_unit_id(int value) {
    unit_id = value;
}

void BirdUnit::set_team_id(int value) {
    team_id = value;
}

void BirdUnit::set_grid_position(const Vector2i &value) {
    grid_position = value;
}

void BirdUnit::set_boid_velocity(const Vector2 &value) {
    velocity = value;
}

void BirdUnit::set_boid_target_pos(const Vector2 &value) {
    boid_target.set_position(value);
}

void BirdUnit::set_boid_data(Ref<BoidData> value) {
    boid_data = value;
}

void BirdUnit::set_boid_logic(Ref<BoidLogic> value) {
    boid_logic = value;
}

void BirdUnit::set_boid_seeking_active(bool value) {
    boid_seeking_active = value;
}
