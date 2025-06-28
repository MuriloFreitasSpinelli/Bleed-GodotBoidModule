#include "BoidData.h"

// Initialize static members
float BoidData::left_border = 0.0f;
float BoidData::right_border = 0.0f;
float BoidData::top_border = 0.0f;
float BoidData::bottom_border = 0.0f;

BoidData::BoidData() {
}

BoidData::~BoidData() {
}

void BoidData::_bind_methods() {
    // Bind exported properties for Godot inspector
    ClassDB::bind_method(D_METHOD("set_world_margin", "value"), &BoidData::set_world_margin);
    ClassDB::bind_method(D_METHOD("get_world_margin"), &BoidData::get_world_margin);
    ClassDB::add_property("BoidData", PropertyInfo(Variant::FLOAT, "world_margin"), "set_world_margin", "get_world_margin");

    ClassDB::bind_method(D_METHOD("set_world_pull_force", "value"), &BoidData::set_world_pull_force);
    ClassDB::bind_method(D_METHOD("get_world_pull_force"), &BoidData::get_world_pull_force);
    ClassDB::add_property("BoidData", PropertyInfo(Variant::FLOAT, "world_pull_force"), "set_world_pull_force", "get_world_pull_force");

    ClassDB::bind_method(D_METHOD("set_cohesion_force", "value"), &BoidData::set_cohesion_force);
    ClassDB::bind_method(D_METHOD("get_cohesion_force"), &BoidData::get_cohesion_force);
    ClassDB::add_property("BoidData", PropertyInfo(Variant::FLOAT, "cohesion_force"), "set_cohesion_force", "get_cohesion_force");

    ClassDB::bind_method(D_METHOD("set_separation_force", "value"), &BoidData::set_separation_force);
    ClassDB::bind_method(D_METHOD("get_separation_force"), &BoidData::get_separation_force);
    ClassDB::add_property("BoidData", PropertyInfo(Variant::FLOAT, "separation_force"), "set_separation_force", "get_separation_force");

    ClassDB::bind_method(D_METHOD("set_alignment_force", "value"), &BoidData::set_alignment_force);
    ClassDB::bind_method(D_METHOD("get_alignment_force"), &BoidData::get_alignment_force);
    ClassDB::add_property("BoidData", PropertyInfo(Variant::FLOAT, "alignment_force"), "set_alignment_force", "get_alignment_force");

    ClassDB::bind_method(D_METHOD("set_chase_force", "value"), &BoidData::set_chase_force);
    ClassDB::bind_method(D_METHOD("get_chase_force"), &BoidData::get_chase_force);
    ClassDB::add_property("BoidData", PropertyInfo(Variant::FLOAT, "chase_force"), "set_chase_force", "get_chase_force");

    ClassDB::bind_method(D_METHOD("set_self_separation_range", "value"), &BoidData::set_self_separation_range);
    ClassDB::bind_method(D_METHOD("get_self_separation_range"), &BoidData::get_self_separation_range);
    ClassDB::add_property("BoidData", PropertyInfo(Variant::FLOAT, "self_separation_range"), "set_self_separation_range", "get_self_separation_range");

    ClassDB::bind_method(D_METHOD("set_combat_range", "value"), &BoidData::set_combat_range);
    ClassDB::bind_method(D_METHOD("get_combat_range"), &BoidData::get_combat_range);
    ClassDB::add_property("BoidData", PropertyInfo(Variant::FLOAT, "combat_range"), "set_combat_range", "get_combat_range");

    ClassDB::bind_method(D_METHOD("set_view_range", "value"), &BoidData::set_view_range);
    ClassDB::bind_method(D_METHOD("get_view_range"), &BoidData::get_view_range);
    ClassDB::add_property("BoidData", PropertyInfo(Variant::FLOAT, "view_range"), "set_view_range", "get_view_range");

    ClassDB::bind_method(D_METHOD("set_view_angle", "value"), &BoidData::set_view_angle);
    ClassDB::bind_method(D_METHOD("get_view_angle"), &BoidData::get_view_angle);
    ClassDB::add_property("BoidData", PropertyInfo(Variant::FLOAT, "view_angle"), "set_view_angle", "get_view_angle");

    ClassDB::bind_method(D_METHOD("set_max_speed", "value"), &BoidData::set_max_speed);
    ClassDB::bind_method(D_METHOD("get_max_speed"), &BoidData::get_max_speed);
    ClassDB::add_property("BoidData", PropertyInfo(Variant::FLOAT, "max_speed"), "set_max_speed", "get_max_speed");

    // Bind static methods
    ClassDB::bind_static_method("BoidData", D_METHOD("recalculate_borders"), &BoidData::recalculate_borders);
    ClassDB::bind_method(D_METHOD("get_data"), &BoidData::get_data);

    // Bind static border methods
    ClassDB::bind_static_method("BoidData", D_METHOD("set_left_border", "value"), &BoidData::set_left_border);
    ClassDB::bind_static_method("BoidData", D_METHOD("get_left_border"), &BoidData::get_left_border);
    ClassDB::bind_static_method("BoidData", D_METHOD("set_right_border", "value"), &BoidData::set_right_border);
    ClassDB::bind_static_method("BoidData", D_METHOD("get_right_border"), &BoidData::get_right_border);
    ClassDB::bind_static_method("BoidData", D_METHOD("set_top_border", "value"), &BoidData::set_top_border);
    ClassDB::bind_static_method("BoidData", D_METHOD("get_top_border"), &BoidData::get_top_border);
    ClassDB::bind_static_method("BoidData", D_METHOD("set_bottom_border", "value"), &BoidData::set_bottom_border);
    ClassDB::bind_static_method("BoidData", D_METHOD("get_bottom_border"), &BoidData::get_bottom_border);
}

// Static methods
void BoidData::recalculate_borders() {
    // Implementation for recalculating borders
    // Add your border calculation logic here
}

// Get data snapshot
BoidDataSnapshot BoidData::get_data() const {
    BoidDataSnapshot snapshot;
    snapshot.world_margin = world_margin;
    snapshot.world_pull_force = world_pull_force;
    snapshot.cohesion_force = cohesion_force;
    snapshot.separation_force = separation_force;
    snapshot.alignment_force = alignment_force;
    snapshot.chase_force = chase_force;
    snapshot.self_separation_range = self_separation_range;
    snapshot.combat_range = combat_range;
    snapshot.view_range = view_range;
    snapshot.view_angle = view_angle;
    snapshot.max_speed = max_speed;
    snapshot.left_border = left_border;
    snapshot.right_border = right_border;
    snapshot.top_border = top_border;
    snapshot.bottom_border = bottom_border;
    return snapshot;
}

// Property getters and setters
void BoidData::set_world_margin(float p_value) {
    world_margin = p_value;
}

float BoidData::get_world_margin() const {
    return world_margin;
}

void BoidData::set_world_pull_force(float p_value) {
    world_pull_force = p_value;
}

float BoidData::get_world_pull_force() const {
    return world_pull_force;
}

void BoidData::set_cohesion_force(float p_value) {
    cohesion_force = p_value;
}

float BoidData::get_cohesion_force() const {
    return cohesion_force;
}

void BoidData::set_separation_force(float p_value) {
    separation_force = p_value;
}

float BoidData::get_separation_force() const {
    return separation_force;
}

void BoidData::set_alignment_force(float p_value) {
    alignment_force = p_value;
}

float BoidData::get_alignment_force() const {
    return alignment_force;
}

void BoidData::set_chase_force(float p_value) {
    chase_force = p_value;
}

float BoidData::get_chase_force() const {
    return chase_force;
}

void BoidData::set_self_separation_range(float p_value) {
    self_separation_range = p_value;
}

float BoidData::get_self_separation_range() const {
    return self_separation_range;
}

void BoidData::set_combat_range(float p_value) {
    combat_range = p_value;
}

float BoidData::get_combat_range() const {
    return combat_range;
}

void BoidData::set_view_range(float p_value) {
    view_range = p_value;
}

float BoidData::get_view_range() const {
    return view_range;
}

void BoidData::set_view_angle(float p_value) {
    view_angle = p_value;
}

float BoidData::get_view_angle() const {
    return view_angle;
}

void BoidData::set_max_speed(float p_value) {
    max_speed = p_value;
}

float BoidData::get_max_speed() const {
    return max_speed;
}

// Static border getters and setters
void BoidData::set_left_border(float p_value) {
    left_border = p_value;
}

float BoidData::get_left_border() {
    return left_border;
}

void BoidData::set_right_border(float p_value) {
    right_border = p_value;
}

float BoidData::get_right_border() {
    return right_border;
}

void BoidData::set_top_border(float p_value) {
    top_border = p_value;
}

float BoidData::get_top_border() {
    return top_border;
}

void BoidData::set_bottom_border(float p_value) {
    bottom_border = p_value;
}

float BoidData::get_bottom_border() {
    return bottom_border;
}