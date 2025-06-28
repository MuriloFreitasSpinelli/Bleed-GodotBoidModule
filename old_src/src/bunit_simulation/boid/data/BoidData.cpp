#include "BoidData.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void BoidData::_bind_methods() {
    // Logic
    ClassDB::bind_method(D_METHOD("get_snapshot"), &BoidData::get_snapshot);

    // --- max_speed ---
    ClassDB::bind_method(D_METHOD("get_max_speed"), &BoidData::get_max_speed);
    ClassDB::bind_method(D_METHOD("set_max_speed", "value"), &BoidData::set_max_speed);
    ClassDB::add_property<BoidData>(
        PropertyInfo(Variant::FLOAT, "max_speed", PROPERTY_HINT_RANGE, "0,1000,0.1"),
        "set_max_speed", "get_max_speed");

    // --- view_range ---
    ClassDB::bind_method(D_METHOD("get_view_range"), &BoidData::get_view_range);
    ClassDB::bind_method(D_METHOD("set_view_range", "value"), &BoidData::set_view_range);
    ClassDB::add_property<BoidData>(
        PropertyInfo(Variant::FLOAT, "view_range", PROPERTY_HINT_RANGE, "0,1000,0.1"),
        "set_view_range", "get_view_range");

    // --- view_angle (radians) ---
    ClassDB::bind_method(D_METHOD("get_view_angle"), &BoidData::get_view_angle);
    ClassDB::bind_method(D_METHOD("set_view_angle", "value"), &BoidData::set_view_angle);
    ClassDB::add_property<BoidData>(
        PropertyInfo(Variant::FLOAT, "view_angle", PROPERTY_HINT_RANGE, "0,6.28319,0.01", PROPERTY_USAGE_DEFAULT, "radians"),
        "set_view_angle", "get_view_angle");

    // --- separation_range ---
    ClassDB::bind_method(D_METHOD("get_separation_range"), &BoidData::get_separation_range);
    ClassDB::bind_method(D_METHOD("set_separation_range", "value"), &BoidData::set_separation_range);
    ClassDB::add_property<BoidData>(
        PropertyInfo(Variant::FLOAT, "separation_range", PROPERTY_HINT_RANGE, "0,1000,0.1"),
        "set_separation_range", "get_separation_range");

    // --- combat_range ---
    ClassDB::bind_method(D_METHOD("get_combat_range"), &BoidData::get_combat_range);
    ClassDB::bind_method(D_METHOD("set_combat_range", "value"), &BoidData::set_combat_range);
    ClassDB::add_property<BoidData>(
        PropertyInfo(Variant::FLOAT, "combat_range", PROPERTY_HINT_RANGE, "0,1000,0.1"),
        "set_combat_range", "get_combat_range");
}

// Optional default constructor giving sensible starting values.
BoidData::BoidData() :
    max_speed(10.0f),
    world_margin(2.0f),
    view_range(20.0f),
    view_angle(Math_PI),
    separation_range(3.0f),
    combat_range(5.0f) {
}
