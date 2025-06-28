#include "unit_step.h"

using namespace godot;

UnitStep::UnitStep() : unique_id(0), hurt(0.0f), heal(0.0f) {
    inflictions = Array();
}

UnitStep::UnitStep(int unique_id, float hurt, float heal, const Array &inflictions) 
    : unique_id(unique_id), hurt(hurt), heal(heal), inflictions(inflictions) {
}

UnitStep::~UnitStep() = default;

void UnitStep::_bind_methods() {
    // Bind constructors
    ClassDB::bind_method(D_METHOD("_init"), &UnitStep::UnitStep);
    
    // Bind getters
    ClassDB::bind_method(D_METHOD("get_unique_id"), &UnitStep::get_unique_id);
    ClassDB::bind_method(D_METHOD("get_hurt"), &UnitStep::get_hurt);
    ClassDB::bind_method(D_METHOD("get_heal"), &UnitStep::get_heal);
    ClassDB::bind_method(D_METHOD("get_inflictions"), &UnitStep::get_inflictions);
    
    // Bind setters
    ClassDB::bind_method(D_METHOD("set_unique_id", "id"), &UnitStep::set_unique_id);
    ClassDB::bind_method(D_METHOD("set_hurt", "h"), &UnitStep::set_hurt);
    ClassDB::bind_method(D_METHOD("set_heal", "h"), &UnitStep::set_heal);
    ClassDB::bind_method(D_METHOD("set_inflictions", "i"), &UnitStep::set_inflictions);
    
    // Bind utility methods
    ClassDB::bind_method(D_METHOD("join_steps", "other"), &UnitStep::join_steps);
    
    // Add properties for Godot inspector
    ADD_PROPERTY(PropertyInfo(Variant::INT, "unique_id"), "set_unique_id", "get_unique_id");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "hurt"), "set_hurt", "get_hurt");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "heal"), "set_heal", "get_heal");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "inflictions"), "set_inflictions", "get_inflictions");
}

void UnitStep::join_steps(const UnitStep &other) {
    // Add numeric values together
    hurt += other.get_hurt();
    heal += other.get_heal();
    
    // Merge inflictions array, avoiding duplicates
    Array other_inflictions = other.get_inflictions();
    
    for (int i = 0; i < other_inflictions.size(); i++) {
        Variant infliction = other_inflictions[i];
        
        // Check if this infliction already exists in our array
        bool already_exists = false;
        for (int j = 0; j < inflictions.size(); j++) {
            if (inflictions[j] == infliction) {
                already_exists = true;
                break;
            }
        }
        
        // Only add if it doesn't already exist
        if (!already_exists) {
            inflictions.append(infliction);
        }
    }
}