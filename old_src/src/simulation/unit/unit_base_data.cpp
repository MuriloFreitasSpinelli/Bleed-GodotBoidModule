#include "unit_base_data.h"

using namespace godot;

UnitBaseData::UnitBaseData() : unit_id(0), base_health(100.0f), base_strength(10.0f), 
                               base_speed(50.0f), base_defense(5.0f) {
    passives = Array();
}

UnitBaseData::~UnitBaseData() = default;

void UnitBaseData::_bind_methods() {
    // Bind getters
    ClassDB::bind_method(D_METHOD("get_unit_id"), &UnitBaseData::get_unit_id);
    ClassDB::bind_method(D_METHOD("get_base_health"), &UnitBaseData::get_base_health);
    ClassDB::bind_method(D_METHOD("get_base_strength"), &UnitBaseData::get_base_strength);
    ClassDB::bind_method(D_METHOD("get_base_speed"), &UnitBaseData::get_base_speed);
    ClassDB::bind_method(D_METHOD("get_base_defense"), &UnitBaseData::get_base_defense);
    ClassDB::bind_method(D_METHOD("get_passives"), &UnitBaseData::get_passives);
    ClassDB::bind_method(D_METHOD("get_trait"), &UnitBaseData::get_trait);
    
    // Bind setters
    ClassDB::bind_method(D_METHOD("set_unit_id", "id"), &UnitBaseData::set_unit_id);
    ClassDB::bind_method(D_METHOD("set_base_health", "health"), &UnitBaseData::set_base_health);
    ClassDB::bind_method(D_METHOD("set_base_strength", "strength"), &UnitBaseData::set_base_strength);
    ClassDB::bind_method(D_METHOD("set_base_speed", "speed"), &UnitBaseData::set_base_speed);
    ClassDB::bind_method(D_METHOD("set_base_defense", "defense"), &UnitBaseData::set_base_defense);
    ClassDB::bind_method(D_METHOD("set_passives", "p"), &UnitBaseData::set_passives);
    ClassDB::bind_method(D_METHOD("set_trait", "t"), &UnitBaseData::set_trait);
    
    // Bind utility methods
    ClassDB::bind_method(D_METHOD("add_passive", "p"), &UnitBaseData::add_passive);
    ClassDB::bind_method(D_METHOD("remove_passive", "p"), &UnitBaseData::remove_passive);
    
    // Add properties for Godot inspector
    ADD_PROPERTY(PropertyInfo(Variant::INT, "unit_id"), "set_unit_id", "get_unit_id");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "base_health", PROPERTY_HINT_RANGE, "1,1000,1"), "set_base_health", "get_base_health");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "base_strength", PROPERTY_HINT_RANGE, "0,100,0.1"), "set_base_strength", "get_base_strength");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "base_speed", PROPERTY_HINT_RANGE, "0,200,1"), "set_base_speed", "get_base_speed");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "base_defense", PROPERTY_HINT_RANGE, "0,100,0.1"), "set_base_defense", "get_base_defense");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "passives", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "Passive")), "set_passives", "get_passives");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "trait", PROPERTY_HINT_RESOURCE_TYPE, "Passive"), "set_trait", "get_trait");
}