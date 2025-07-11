#include "boid.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>


Boid::Boid() {
    index = 0;
    unit_id = 0;
    velocity = Vector2();
    position = Vector2();
    attack_vector = Vector2();

    linear_damp = 0;
    neighbor_indexes = PackedInt32Array();
    sleepy = 0;

    team = WHITE_TEAM;
    layer = ONE;

    view_range = 0.0f;
    combat_range = 0.0f;
    collision_range = 0.0f;
    view_angle = 0.0f;

    alignment_mask = 0;
    separation_mask = 0;
    cohesion_mask = 0;

    alignment_strength_self = 0.0f;
    separation_strength_self = 0.0f;
    cohesion_strength_self = 0.0f;

    alignment_strength_others = 0.0f;
    separation_strength_others = 0.0f;
    cohesion_strength_others = 0.0f;

    speed = 0.0f;
    max_speed = 0.0f;
}

Boid::~Boid() {
    // Destructor
}

void Boid::_bind_methods() {
    // Bind getters and setters
    ClassDB::bind_method(D_METHOD("get_index"), &Boid::get_index);
    ClassDB::bind_method(D_METHOD("set_index", "value"), &Boid::set_index);
    
    ClassDB::bind_method(D_METHOD("get_unit_id"), &Boid::get_unit_id);
    ClassDB::bind_method(D_METHOD("set_unit_id", "value"), &Boid::set_unit_id);
    
    ClassDB::bind_method(D_METHOD("get_velocity"), &Boid::get_velocity);
    ClassDB::bind_method(D_METHOD("set_velocity", "value"), &Boid::set_velocity);
    
    ClassDB::bind_method(D_METHOD("get_position"), &Boid::get_position);
    ClassDB::bind_method(D_METHOD("set_position", "value"), &Boid::set_position);
    
    ClassDB::bind_method(D_METHOD("get_attack_vector"), &Boid::get_attack_vector);
    ClassDB::bind_method(D_METHOD("set_attack_vector", "value"), &Boid::set_attack_vector);
    
    ClassDB::bind_method(D_METHOD("get_linear_damp"), &Boid::get_linear_damp);
    ClassDB::bind_method(D_METHOD("set_linear_damp", "value"), &Boid::set_linear_damp);
    
    ClassDB::bind_method(D_METHOD("get_neighbor_indexes"), &Boid::get_neighbor_indexes);
    ClassDB::bind_method(D_METHOD("set_neighbor_indexes", "value"), &Boid::set_neighbor_indexes);
    
    ClassDB::bind_method(D_METHOD("get_sleepy"), &Boid::get_sleepy);
    ClassDB::bind_method(D_METHOD("set_sleepy", "value"), &Boid::set_sleepy);
    
    ClassDB::bind_method(D_METHOD("get_team"), &Boid::get_team);
    ClassDB::bind_method(D_METHOD("set_team", "value"), &Boid::set_team);
    
    ClassDB::bind_method(D_METHOD("get_layer"), &Boid::get_layer);
    ClassDB::bind_method(D_METHOD("set_layer", "value"), &Boid::set_layer);
    
    ClassDB::bind_method(D_METHOD("get_view_range"), &Boid::get_view_range);
    ClassDB::bind_method(D_METHOD("set_view_range", "value"), &Boid::set_view_range);
    
    ClassDB::bind_method(D_METHOD("get_combat_range"), &Boid::get_combat_range);
    ClassDB::bind_method(D_METHOD("set_combat_range", "value"), &Boid::set_combat_range);
    
    ClassDB::bind_method(D_METHOD("get_collision_range"), &Boid::get_collision_range);
    ClassDB::bind_method(D_METHOD("set_collision_range", "value"), &Boid::set_collision_range);
    
    ClassDB::bind_method(D_METHOD("get_view_angle"), &Boid::get_view_angle);
    ClassDB::bind_method(D_METHOD("set_view_angle", "value"), &Boid::set_view_angle);
    
    ClassDB::bind_method(D_METHOD("get_alignment_mask"), &Boid::get_alignment_mask);
    ClassDB::bind_method(D_METHOD("set_alignment_mask", "value"), &Boid::set_alignment_mask);
    
    ClassDB::bind_method(D_METHOD("get_separation_mask"), &Boid::get_separation_mask);
    ClassDB::bind_method(D_METHOD("set_separation_mask", "value"), &Boid::set_separation_mask);
    
    ClassDB::bind_method(D_METHOD("get_cohesion_mask"), &Boid::get_cohesion_mask);
    ClassDB::bind_method(D_METHOD("set_cohesion_mask", "value"), &Boid::set_cohesion_mask);
    
    ClassDB::bind_method(D_METHOD("get_alignment_strength_self"), &Boid::get_alignment_strength_self);
    ClassDB::bind_method(D_METHOD("set_alignment_strength_self", "value"), &Boid::set_alignment_strength_self);
    
    ClassDB::bind_method(D_METHOD("get_separation_strength_self"), &Boid::get_separation_strength_self);
    ClassDB::bind_method(D_METHOD("set_separation_strength_self", "value"), &Boid::set_separation_strength_self);
    
    ClassDB::bind_method(D_METHOD("get_cohesion_strength_self"), &Boid::get_cohesion_strength_self);
    ClassDB::bind_method(D_METHOD("set_cohesion_strength_self", "value"), &Boid::set_cohesion_strength_self);
    
    ClassDB::bind_method(D_METHOD("get_alignment_strength_others"), &Boid::get_alignment_strength_others);
    ClassDB::bind_method(D_METHOD("set_alignment_strength_others", "value"), &Boid::set_alignment_strength_others);
    
    ClassDB::bind_method(D_METHOD("get_separation_strength_others"), &Boid::get_separation_strength_others);
    ClassDB::bind_method(D_METHOD("set_separation_strength_others", "value"), &Boid::set_separation_strength_others);
    
    ClassDB::bind_method(D_METHOD("get_cohesion_strength_others"), &Boid::get_cohesion_strength_others);
    ClassDB::bind_method(D_METHOD("set_cohesion_strength_others", "value"), &Boid::set_cohesion_strength_others);
    
    ClassDB::bind_method(D_METHOD("get_speed"), &Boid::get_speed);
    ClassDB::bind_method(D_METHOD("set_speed", "value"), &Boid::set_speed);
    
    ClassDB::bind_method(D_METHOD("get_max_speed"), &Boid::get_max_speed);
    ClassDB::bind_method(D_METHOD("set_max_speed", "value"), &Boid::set_max_speed);
    
    // Bind update method
    ClassDB::bind_method(D_METHOD("update_boid", "delta"), &Boid::update_boid);
    


    ADD_GROUP("Core Identity", "");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "unit_id"), "set_unit_id", "get_unit_id");

    ADD_GROUP("Movement", "");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "velocity"), "set_velocity", "get_velocity");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "position"), "set_position", "get_position");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "attack_vector"), "set_attack_vector", "get_attack_vector");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "linear_damp"), "set_linear_damp", "get_linear_damp");

    ADD_GROUP("State", "");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "sleepy"), "set_sleepy", "get_sleepy");

    ADD_GROUP("Exported Properties", "");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "team"), "set_team", "get_team");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "layer"), "set_layer", "get_layer");

    ADD_GROUP("Ranges", "");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "view_range"), "set_view_range", "get_view_range");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "combat_range"), "set_combat_range", "get_combat_range");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "collision_range"), "set_collision_range", "get_collision_range");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "view_angle"), "set_view_angle", "get_view_angle");

    ADD_GROUP("Behavior Masks", "");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "alignment_mask"), "set_alignment_mask", "get_alignment_mask");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "separation_mask"), "set_separation_mask", "get_separation_mask");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "cohesion_mask"), "set_cohesion_mask", "get_cohesion_mask");

    ADD_GROUP("Self Strengths", "");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "alignment_strength_self"), "set_alignment_strength_self", "get_alignment_strength_self");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "separation_strength_self"), "set_separation_strength_self", "get_separation_strength_self");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cohesion_strength_self"), "set_cohesion_strength_self", "get_cohesion_strength_self");

    ADD_GROUP("Other Strengths", "");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "alignment_strength_others"), "set_alignment_strength_others", "get_alignment_strength_others");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "separation_strength_others"), "set_separation_strength_others", "get_separation_strength_others");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cohesion_strength_others"), "set_cohesion_strength_others", "get_cohesion_strength_others");

    ADD_GROUP("Speed", "");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_speed"), "set_max_speed", "get_max_speed");

    
    // Add signals (for rarely changed variables, excluding index, unit_id, velocity, position)
    ADD_SIGNAL(MethodInfo("attack_vector_changed", PropertyInfo(Variant::VECTOR2, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("linear_damp_changed", PropertyInfo(Variant::FLOAT, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("sleepy_changed", PropertyInfo(Variant::INT, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("view_range_changed", PropertyInfo(Variant::FLOAT, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("combat_range_changed", PropertyInfo(Variant::FLOAT, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("collision_range_changed", PropertyInfo(Variant::FLOAT, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("view_angle_changed", PropertyInfo(Variant::FLOAT, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("alignment_mask_changed", PropertyInfo(Variant::INT, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("separation_mask_changed", PropertyInfo(Variant::INT, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("cohesion_mask_changed", PropertyInfo(Variant::INT, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("alignment_strength_self_changed", PropertyInfo(Variant::FLOAT, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("separation_strength_self_changed", PropertyInfo(Variant::FLOAT, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("cohesion_strength_self_changed", PropertyInfo(Variant::FLOAT, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("alignment_strength_others_changed", PropertyInfo(Variant::FLOAT, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("separation_strength_others_changed", PropertyInfo(Variant::FLOAT, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("cohesion_strength_others_changed", PropertyInfo(Variant::FLOAT, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("speed_changed", PropertyInfo(Variant::FLOAT, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("max_speed_changed", PropertyInfo(Variant::FLOAT, "new_value"), PropertyInfo(Variant::INT, "index")));

    // Bind constants
    BIND_CONSTANT(WHITE_TEAM);
    BIND_CONSTANT(BLACK_TEAM);
    BIND_CONSTANT(RED_TEAM);
    BIND_CONSTANT(ONE);
    BIND_CONSTANT(TWO);
    BIND_CONSTANT(THREE);
    BIND_CONSTANT(FOUR);
    BIND_CONSTANT(FIVE);
    BIND_CONSTANT(VIEW_RANGE);
    BIND_CONSTANT(COMBAT_RANGE);
    BIND_CONSTANT(COLLISION_RANGE);
    BIND_CONSTANT(VIEW_ANGLE);
}

void Boid::update_boid(float delta) {
    position += velocity * ((1 - linear_damp) * delta) * speed;
}