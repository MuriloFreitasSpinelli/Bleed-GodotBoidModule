// BoidRule.cpp
#include "BoidRule.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/property_info.hpp>
using namespace godot;

void BoidRule::_bind_methods() {
    // --------------------------------------------------------------------
    // Expose range flag constants
    BIND_ENUM_CONSTANT(RANGE_IN_VIEW);
    BIND_ENUM_CONSTANT(RANGE_IN_COMBAT);

    // Expose layer flag constants
    BIND_ENUM_CONSTANT(LAYER_GROUND);
    BIND_ENUM_CONSTANT(LAYER_FLYING);

    // Expose team flag constants
    BIND_ENUM_CONSTANT(TEAM_SPLAT);
    BIND_ENUM_CONSTANT(TEAM_ABYSS);
    BIND_ENUM_CONSTANT(TEAM_CHAOS);

    // --------------------------------------------------------------------
    // Property: Range Flags
    ClassDB::bind_method(D_METHOD("get_range_flags"), &BoidRule::get_range_flags);
    ClassDB::bind_method(D_METHOD("set_range_flags", "flags"), &BoidRule::set_range_flags);
    ClassDB::bind_method(D_METHOD("has_range_flag", "flag"), &BoidRule::has_range_flag);
    ClassDB::bind_method(D_METHOD("add_range_flag", "flag"), &BoidRule::add_range_flag);
    ClassDB::bind_method(D_METHOD("remove_range_flag", "flag"), &BoidRule::remove_range_flag);

    ClassDB::add_property<BoidRule>(
        PropertyInfo(Variant::INT, "range_flags",
            PROPERTY_HINT_FLAGS, "IN_VIEW,IN_COMBAT"),
        "set_range_flags", "get_range_flags");

    // Property: Layer Flags
    ClassDB::bind_method(D_METHOD("get_layer_flags"), &BoidRule::get_layer_flags);
    ClassDB::bind_method(D_METHOD("set_layer_flags", "flags"), &BoidRule::set_layer_flags);
    ClassDB::bind_method(D_METHOD("has_layer_flag", "flag"), &BoidRule::has_layer_flag);
    ClassDB::bind_method(D_METHOD("add_layer_flag", "flag"), &BoidRule::add_layer_flag);
    ClassDB::bind_method(D_METHOD("remove_layer_flag", "flag"), &BoidRule::remove_layer_flag);

    ClassDB::add_property<BoidRule>(
        PropertyInfo(Variant::INT, "layer_flags",
            PROPERTY_HINT_FLAGS, "GROUND,FLYING"),
        "set_layer_flags", "get_layer_flags");

    // Property: Team Flags
    ClassDB::bind_method(D_METHOD("get_team_flags"), &BoidRule::get_team_flags);
    ClassDB::bind_method(D_METHOD("set_team_flags", "flags"), &BoidRule::set_team_flags);
    ClassDB::bind_method(D_METHOD("has_team_flag", "flag"), &BoidRule::has_team_flag);
    ClassDB::bind_method(D_METHOD("add_team_flag", "flag"), &BoidRule::add_team_flag);
    ClassDB::bind_method(D_METHOD("remove_team_flag", "flag"), &BoidRule::remove_team_flag);

    ClassDB::add_property<BoidRule>(
        PropertyInfo(Variant::INT, "team_flags",
            PROPERTY_HINT_FLAGS, "SPLAT,ABYSS,CHAOS"),
        "set_team_flags", "get_team_flags");
}
