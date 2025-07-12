#include "boid.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <algorithm> // std::clamp

using namespace godot;

Boid::Boid() = default;
Boid::~Boid() = default;

void Boid::_bind_methods() {
    // ── Property bindings ──
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

    ClassDB::bind_method(D_METHOD("update_boid", "delta"), &Boid::update_boid);

    // ── Signals ──
    ADD_SIGNAL(MethodInfo("attack_vector_changed", PropertyInfo(Variant::VECTOR2, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("linear_damp_changed", PropertyInfo(Variant::FLOAT, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("sleepy_changed", PropertyInfo(Variant::INT, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("team_changed", PropertyInfo(Variant::INT, "new_value"), PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("layer_changed", PropertyInfo(Variant::INT, "new_value"), PropertyInfo(Variant::INT, "index")));
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

    // ── Constants ──
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

// ────────────────────────────────────────────────────────────
//  Setter implementations that need signal emission
// ────────────────────────────────────────────────────────────
#define SET_AND_EMIT(var, valueName, signalName) \
    var = valueName;                              \
    emit_signal(signalName, var, index)

void Boid::set_attack_vector(const Vector2& p_value) { SET_AND_EMIT(attack_vector, p_value, "attack_vector_changed"); }
void Boid::set_linear_damp(real_t p_value) { SET_AND_EMIT(linear_damp, p_value, "linear_damp_changed"); }
void Boid::set_sleepy(uint32_t p_value) { SET_AND_EMIT(sleepy, p_value, "sleepy_changed"); }
void Boid::set_team(uint32_t p_value) { SET_AND_EMIT(team, p_value, "team_changed"); }
void Boid::set_layer(uint32_t p_value) { SET_AND_EMIT(layer, p_value, "layer_changed"); }
void Boid::set_view_range(real_t p_value) { SET_AND_EMIT(view_range, p_value, "view_range_changed"); }
void Boid::set_combat_range(real_t p_value) { SET_AND_EMIT(combat_range, p_value, "combat_range_changed"); }
void Boid::set_collision_range(real_t p_value) { SET_AND_EMIT(collision_range, p_value, "collision_range_changed"); }
void Boid::set_view_angle(real_t p_value) { SET_AND_EMIT(view_angle, p_value, "view_angle_changed"); }
void Boid::set_alignment_mask(uint32_t p_value) { SET_AND_EMIT(alignment_mask, p_value, "alignment_mask_changed"); }
void Boid::set_separation_mask(uint32_t p_value) { SET_AND_EMIT(separation_mask, p_value, "separation_mask_changed"); }
void Boid::set_cohesion_mask(uint32_t p_value) { SET_AND_EMIT(cohesion_mask, p_value, "cohesion_mask_changed"); }
void Boid::set_alignment_strength_self(real_t p_value) { SET_AND_EMIT(alignment_strength_self, p_value, "alignment_strength_self_changed"); }
void Boid::set_separation_strength_self(real_t p_value) { SET_AND_EMIT(separation_strength_self, p_value, "separation_strength_self_changed"); }
void Boid::set_cohesion_strength_self(real_t p_value) { SET_AND_EMIT(cohesion_strength_self, p_value, "cohesion_strength_self_changed"); }
void Boid::set_alignment_strength_others(real_t p_value) { SET_AND_EMIT(alignment_strength_others, p_value, "alignment_strength_others_changed"); }
void Boid::set_separation_strength_others(real_t p_value) { SET_AND_EMIT(separation_strength_others, p_value, "separation_strength_others_changed"); }
void Boid::set_cohesion_strength_others(real_t p_value) { SET_AND_EMIT(cohesion_strength_others, p_value, "cohesion_strength_others_changed"); }
void Boid::set_speed(real_t p_value) { SET_AND_EMIT(speed, p_value, "speed_changed"); }
void Boid::set_max_speed(real_t p_value) { SET_AND_EMIT(max_speed, p_value, "max_speed_changed"); }
#undef SET_AND_EMIT

// ────────────────────────────────────────────────────────────
//  Main update
// ────────────────────────────────────────────────────────────
void Boid::update_boid(double p_delta) {
    const real_t delta = static_cast<real_t>(p_delta);

    // Integrate position
    position += velocity * speed * delta;

    // Apply damping (clamped 0..1)
    velocity *= 1.0 - std::clamp(linear_damp * delta, static_cast<real_t>(0.0), static_cast<real_t>(1.0));

    // Clamp to max_speed, if > 0
    if (max_speed > 0.0 && velocity.length() > max_speed) {
        velocity = velocity.normalized() * max_speed;
    }
}
