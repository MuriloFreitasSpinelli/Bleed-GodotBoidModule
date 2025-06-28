#include "BoidSnapshot.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/classes/global_constants.hpp>

using namespace godot;

void BoidSnapshot::_bind_methods() {
    // --- unique_id ---
    ClassDB::bind_method(D_METHOD("get_unique_id"), &BoidSnapshot::get_unique_id);
    ClassDB::bind_method(D_METHOD("set_unique_id", "value"), &BoidSnapshot::set_unique_id);

    // --- layer_id ---
    ClassDB::bind_method(D_METHOD("get_layer_id"), &BoidSnapshot::get_layer_id);
    ClassDB::bind_method(D_METHOD("set_layer_id", "value"), &BoidSnapshot::set_layer_id);


    // --- team_id ---
    ClassDB::bind_method(D_METHOD("get_team_id"), &BoidSnapshot::get_team_id);
    ClassDB::bind_method(D_METHOD("set_team_id", "value"), &BoidSnapshot::set_team_id);


    // --- position ---
    ClassDB::bind_method(D_METHOD("get_position"), &BoidSnapshot::get_position);
    ClassDB::bind_method(D_METHOD("set_position", "value"), &BoidSnapshot::set_position);


    // --- velocity ---
    ClassDB::bind_method(D_METHOD("get_velocity"), &BoidSnapshot::get_velocity);
    ClassDB::bind_method(D_METHOD("set_velocity", "value"), &BoidSnapshot::set_velocity);


    // --- view_angle ---
    ClassDB::bind_method(D_METHOD("get_view_angle"), &BoidSnapshot::get_view_angle);
    ClassDB::bind_method(D_METHOD("set_view_angle", "value"), &BoidSnapshot::set_view_angle);


    // --- view_range ---
    ClassDB::bind_method(D_METHOD("get_view_range"), &BoidSnapshot::get_view_range);
    ClassDB::bind_method(D_METHOD("set_view_range", "value"), &BoidSnapshot::set_view_range);


    // --- combat_range ---
    ClassDB::bind_method(D_METHOD("get_combat_range"), &BoidSnapshot::get_combat_range);
    ClassDB::bind_method(D_METHOD("set_combat_range", "value"), &BoidSnapshot::set_combat_range);

    // --- separation_range ---
    ClassDB::bind_method(D_METHOD("get_separation_range"), &BoidSnapshot::get_separation_range);
    ClassDB::bind_method(D_METHOD("set_separation_range", "value"), &BoidSnapshot::set_separation_range);

    // --- max_speed ---
    ClassDB::bind_method(D_METHOD("get_max_speed"), &BoidSnapshot::get_max_speed);
    ClassDB::bind_method(D_METHOD("set_max_speed", "value"), &BoidSnapshot::set_max_speed);
}

// --------------------------------------------------------------------------
// Constructors
// --------------------------------------------------------------------------
BoidSnapshot::BoidSnapshot() :
    unique_id(0),
    layer_id(0),
    team_id(0),
    position(Vector2()),
    velocity(Vector2()),
    view_angle(Math_PI),
    view_range(20.0f),
    combat_range(5.0f),
    separation_range(3.0f),
    max_speed(10.0f) {
}

BoidSnapshot::BoidSnapshot(int uid, int lid, int tid, Vector2 pos, Vector2 vel,
    float v_angle, float v_range, float c_range, float s_range, float m_speed) :
    unique_id(uid),
    layer_id(lid),
    team_id(tid),
    position(pos),
    velocity(vel),
    view_angle(v_angle),
    view_range(v_range),
    combat_range(c_range),
    separation_range(s_range),
    max_speed(m_speed) {
}