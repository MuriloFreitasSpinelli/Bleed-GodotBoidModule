#include "unit_data.h"

using namespace godot;

UnitData::UnitData() : health(100.0f), strength(10.0f), speed(50.0f), defense(5.0f), 
                       attack_radius(100), view_radius(150), n_attack_target(1) {
}

UnitData::~UnitData() = default;

void UnitData::_bind_methods() {
    // Bind getters
    ClassDB::bind_method(D_METHOD("get_health"), &UnitData::get_health);
    ClassDB::bind_method(D_METHOD("get_strength"), &UnitData::get_strength);
    ClassDB::bind_method(D_METHOD("get_speed"), &UnitData::get_speed);
    ClassDB::bind_method(D_METHOD("get_defense"), &UnitData::get_defense);
    ClassDB::bind_method(D_METHOD("get_attack_radius"), &UnitData::get_attack_radius);
    ClassDB::bind_method(D_METHOD("get_view_radius"), &UnitData::get_view_radius);
    ClassDB::bind_method(D_METHOD("get_n_attack_target"), &UnitData::get_n_attack_target);
    ClassDB::bind_method(D_METHOD("get_base_data"), &UnitData::get_base_data);
    
    // Bind setters
    ClassDB::bind_method(D_METHOD("set_health", "health"), &UnitData::set_health);
    ClassDB::bind_method(D_METHOD("set_strength", "strength"), &UnitData::set_strength);
    ClassDB::bind_method(D_METHOD("set_speed", "speed"), &UnitData::set_speed);
    ClassDB::bind_method(D_METHOD("set_defense", "defense"), &UnitData::set_defense);
    ClassDB::bind_method(D_METHOD("set_attack_radius", "radius"), &UnitData::set_attack_radius);
    ClassDB::bind_method(D_METHOD("set_view_radius", "radius"), &UnitData::set_view_radius);
    ClassDB::bind_method(D_METHOD("set_n_attack_target", "n"), &UnitData::set_n_attack_target);
    ClassDB::bind_method(D_METHOD("set_base_data", "base_data"), &UnitData::set_base_data);
    
    // Add properties for Godot inspector
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "health", PROPERTY_HINT_RANGE, "0,1000,1"), "set_health", "get_health");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "strength", PROPERTY_HINT_RANGE, "0,100,0.1"), "set_strength", "get_strength");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed", PROPERTY_HINT_RANGE, "0,200,1"), "set_speed", "get_speed");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "defense", PROPERTY_HINT_RANGE, "0,100,0.1"), "set_defense", "get_defense");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "attack_radius", PROPERTY_HINT_RANGE, "0,500,1"), "set_attack_radius", "get_attack_radius");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "view_radius", PROPERTY_HINT_RANGE, "0,1000,1"), "set_view_radius", "get_view_radius");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "n_attack_target", PROPERTY_HINT_RANGE, "1,10,1"), "set_n_attack_target", "get_n_attack_target");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "base_data", PROPERTY_HINT_RESOURCE_TYPE, "UnitBaseData"), "set_base_data", "get_base_data");
}