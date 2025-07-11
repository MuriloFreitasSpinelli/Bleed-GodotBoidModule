#include "boids_handler.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <algorithm>

using namespace godot;

BoidsHandler::BoidsHandler() {
    // Constructor
}

BoidsHandler::~BoidsHandler() {
    // Destructor
}

void BoidsHandler::_bind_methods() {
    ClassDB::bind_method(D_METHOD("register_boid", "boid"), &BoidsHandler::register_boid);
    ClassDB::bind_method(D_METHOD("retire_boid", "boid"), &BoidsHandler::retire_boid);
    ClassDB::bind_method(D_METHOD("batch_register_boids", "boids"), &BoidsHandler::batch_register_boids);
    ClassDB::bind_method(D_METHOD("batch_retire_boids", "boids"), &BoidsHandler::batch_retire_boids);
    
    ClassDB::bind_method(D_METHOD("update_buffer"), &BoidsHandler::update_buffer);
    ClassDB::bind_method(D_METHOD("get_boids_buffer_in_bytes"), &BoidsHandler::get_boids_buffer_in_bytes);

    ClassDB::bind_method(D_METHOD("update_boids", "delta"), &BoidsHandler::update_boids);
    ClassDB::bind_method(D_METHOD("update_boids_velocity", "new_velocities"), &BoidsHandler::update_boids_velocity);
    ClassDB::bind_method(D_METHOD("update_boids_neighbors", "new_neighbors"), &BoidsHandler::update_boids_neighbors);
    
    ClassDB::bind_method(D_METHOD("get_boid", "index"), &BoidsHandler::get_boid);
    ClassDB::bind_method(D_METHOD("get_boid_count"), &BoidsHandler::get_boid_count);
    
    ClassDB::bind_method(D_METHOD("print_debug"), &BoidsHandler::print_boids_debug);
}

void BoidsHandler::register_boid(Boid* boid) {
    if (!boid) return;

    //Already added. Cheap, easy and dirty method...
    if (boid->is_connected("view_range_changed", callable_mp(this, &BoidsHandler::_on_view_range_changed))) {
        return;
    }

    int index;
    if (!retired_boid_indexes.empty()) {
        // Reuse a retired index
        index = retired_boid_indexes.back();
        retired_boid_indexes.pop_back();
        registered_boids[index] = boid;
    }
    else {
        // Add new boid
        index = registered_boids.size();
        registered_boids.push_back(boid);

        // Resize all buffers
        boids_buffer.unit_ids.resize(registered_boids.size());
        boids_buffer.attributes.resize(registered_boids.size());
        boids_buffer.active.resize(registered_boids.size());
        boids_buffer.positions.resize(registered_boids.size());
        boids_buffer.velocities.resize(registered_boids.size());
        boids_buffer.attack_vectors.resize(registered_boids.size());
        boids_buffer.sleepy.resize(registered_boids.size());
        boids_buffer.view_range.resize(registered_boids.size());
        boids_buffer.combat_range.resize(registered_boids.size());
        boids_buffer.collision_range.resize(registered_boids.size());
        boids_buffer.view_angle.resize(registered_boids.size());
        boids_buffer.alignment_mask.resize(registered_boids.size());
        boids_buffer.separation_mask.resize(registered_boids.size());
        boids_buffer.cohesion_mask.resize(registered_boids.size());
        boids_buffer.alignment_strength_self.resize(registered_boids.size());
        boids_buffer.separation_strength_self.resize(registered_boids.size());
        boids_buffer.cohesion_strength_self.resize(registered_boids.size());
        boids_buffer.alignment_strength_others.resize(registered_boids.size());
        boids_buffer.separation_strength_others.resize(registered_boids.size());
        boids_buffer.cohesion_strength_others.resize(registered_boids.size());
        boids_buffer.speed.resize(registered_boids.size());
        boids_buffer.max_speed.resize(registered_boids.size());
    }

    boids_buffer.active[index] = 1;
    boid->set_index(index);

    // Connect signals
    boid->connect("attack_vector_changed", callable_mp(this, &BoidsHandler::_on_attack_vector_changed));
    boid->connect("sleepy_changed", callable_mp(this, &BoidsHandler::_on_sleepy_changed));
    boid->connect("view_range_changed", callable_mp(this, &BoidsHandler::_on_view_range_changed));
    boid->connect("combat_range_changed", callable_mp(this, &BoidsHandler::_on_combat_range_changed));
    boid->connect("collision_range_changed", callable_mp(this, &BoidsHandler::_on_collision_range_changed));
    boid->connect("view_angle_changed", callable_mp(this, &BoidsHandler::_on_view_angle_changed));
    boid->connect("alignment_mask_changed", callable_mp(this, &BoidsHandler::_on_alignment_mask_changed));
    boid->connect("separation_mask_changed", callable_mp(this, &BoidsHandler::_on_separation_mask_changed));
    boid->connect("cohesion_mask_changed", callable_mp(this, &BoidsHandler::_on_cohesion_mask_changed));
    boid->connect("alignment_strength_self_changed", callable_mp(this, &BoidsHandler::_on_alignment_strength_self_changed));
    boid->connect("separation_strength_self_changed", callable_mp(this, &BoidsHandler::_on_separation_strength_self_changed));
    boid->connect("cohesion_strength_self_changed", callable_mp(this, &BoidsHandler::_on_cohesion_strength_self_changed));
    boid->connect("alignment_strength_others_changed", callable_mp(this, &BoidsHandler::_on_alignment_strength_others_changed));
    boid->connect("separation_strength_others_changed", callable_mp(this, &BoidsHandler::_on_separation_strength_others_changed));
    boid->connect("cohesion_strength_others_changed", callable_mp(this, &BoidsHandler::_on_cohesion_strength_others_changed));
    boid->connect("speed_changed", callable_mp(this, &BoidsHandler::_on_speed_changed));
    boid->connect("max_speed_changed", callable_mp(this, &BoidsHandler::_on_max_speed_changed));
    
    // Initial buffer update for this boid
    _update_single_boid(index);
}

void BoidsHandler::retire_boid(int boid_index) {
    if (boid_index < 0 || boid_index >= registered_boids.size()) return;
    Boid* boid = registered_boids[boid_index];
    if (boid_index >= 0 && boid_index < registered_boids.size() && registered_boids[boid_index] == boid) {
        // Disconnect signals
        if (boid->is_connected("attack_vector_changed", callable_mp(this, &BoidsHandler::_on_attack_vector_changed))) {
            boid->disconnect("attack_vector_changed", callable_mp(this, &BoidsHandler::_on_attack_vector_changed));
        }
        if (boid->is_connected("sleepy_changed", callable_mp(this, &BoidsHandler::_on_sleepy_changed))) {
            boid->disconnect("sleepy_changed", callable_mp(this, &BoidsHandler::_on_sleepy_changed));
        }
        if (boid->is_connected("view_range_changed", callable_mp(this, &BoidsHandler::_on_view_range_changed))) {
            boid->disconnect("view_range_changed", callable_mp(this, &BoidsHandler::_on_view_range_changed));
        }
        if (boid->is_connected("combat_range_changed", callable_mp(this, &BoidsHandler::_on_combat_range_changed))) {
            boid->disconnect("combat_range_changed", callable_mp(this, &BoidsHandler::_on_combat_range_changed));
        }
        if (boid->is_connected("collision_range_changed", callable_mp(this, &BoidsHandler::_on_collision_range_changed))) {
            boid->disconnect("collision_range_changed", callable_mp(this, &BoidsHandler::_on_collision_range_changed));
        }
        if (boid->is_connected("view_angle_changed", callable_mp(this, &BoidsHandler::_on_view_angle_changed))) {
            boid->disconnect("view_angle_changed", callable_mp(this, &BoidsHandler::_on_view_angle_changed));
        }
        if (boid->is_connected("alignment_mask_changed", callable_mp(this, &BoidsHandler::_on_alignment_mask_changed))) {
            boid->disconnect("alignment_mask_changed", callable_mp(this, &BoidsHandler::_on_alignment_mask_changed));
        }
        if (boid->is_connected("separation_mask_changed", callable_mp(this, &BoidsHandler::_on_separation_mask_changed))) {
            boid->disconnect("separation_mask_changed", callable_mp(this, &BoidsHandler::_on_separation_mask_changed));
        }
        if (boid->is_connected("cohesion_mask_changed", callable_mp(this, &BoidsHandler::_on_cohesion_mask_changed))) {
            boid->disconnect("cohesion_mask_changed", callable_mp(this, &BoidsHandler::_on_cohesion_mask_changed));
        }
        if (boid->is_connected("alignment_strength_self_changed", callable_mp(this, &BoidsHandler::_on_alignment_strength_self_changed))) {
            boid->disconnect("alignment_strength_self_changed", callable_mp(this, &BoidsHandler::_on_alignment_strength_self_changed));
        }
        if (boid->is_connected("separation_strength_self_changed", callable_mp(this, &BoidsHandler::_on_separation_strength_self_changed))) {
            boid->disconnect("separation_strength_self_changed", callable_mp(this, &BoidsHandler::_on_separation_strength_self_changed));
        }
        if (boid->is_connected("cohesion_strength_self_changed", callable_mp(this, &BoidsHandler::_on_cohesion_strength_self_changed))) {
            boid->disconnect("cohesion_strength_self_changed", callable_mp(this, &BoidsHandler::_on_cohesion_strength_self_changed));
        }
        if (boid->is_connected("alignment_strength_others_changed", callable_mp(this, &BoidsHandler::_on_alignment_strength_others_changed))) {
            boid->disconnect("alignment_strength_others_changed", callable_mp(this, &BoidsHandler::_on_alignment_strength_others_changed));
        }
        if (boid->is_connected("separation_strength_others_changed", callable_mp(this, &BoidsHandler::_on_separation_strength_others_changed))) {
            boid->disconnect("separation_strength_others_changed", callable_mp(this, &BoidsHandler::_on_separation_strength_others_changed));
        }
        if (boid->is_connected("cohesion_strength_others_changed", callable_mp(this, &BoidsHandler::_on_cohesion_strength_others_changed))) {
            boid->disconnect("cohesion_strength_others_changed", callable_mp(this, &BoidsHandler::_on_cohesion_strength_others_changed));
        }
        if (boid->is_connected("speed_changed", callable_mp(this, &BoidsHandler::_on_speed_changed))) {
            boid->disconnect("speed_changed", callable_mp(this, &BoidsHandler::_on_speed_changed));
        }
        if (boid->is_connected("max_speed_changed", callable_mp(this, &BoidsHandler::_on_max_speed_changed))) {
            boid->disconnect("max_speed_changed", callable_mp(this, &BoidsHandler::_on_max_speed_changed));
        }

        boids_buffer.active[boid_index] = 0;
        registered_boids[boid_index] = nullptr;
        retired_boid_indexes.push_back(boid_index);
    }
}

void BoidsHandler::update_buffer() {
    _update_positions();
    _update_velocities();
}

void BoidsHandler::batch_register_boids(TypedArray<Boid> boids) {
    for (int i = 0; i < boids.size(); i++) {
        Boid* boid = Object::cast_to<Boid>(boids[i]);
        if (boid) {
            register_boid(boid);
        }
    }
}

void BoidsHandler::batch_retire_boids(TypedArray<uint32_t> boids_indexes) {
    for (int i = 0; i < boids_indexes.size(); i++) {
        retire_boid(boids_indexes[i]);
    }
}

void BoidsHandler::update_boids(double delta)
{
    for (int i = 0; i < registered_boids.size(); i++) {
        if (registered_boids[i]) {
            registered_boids[i]->update_boid(delta);
        }
    }
}

void BoidsHandler::update_boids_velocity(PackedVector2Array new_velocities) {
    for (int i = 0; i < registered_boids.size(); i++) {
        if (registered_boids[i]) {
            registered_boids[i]->set_velocity(new_velocities[i]);
        }
    }
}

void BoidsHandler::update_boids_neighbors(TypedArray<PackedInt32Array> new_neighbors)
{
    for (int i = 0; i < registered_boids.size(); i++) {
        if (registered_boids[i]) {
            registered_boids[i]->set_neighbor_indexes(new_neighbors[i]);
        }
    }
}

Boid* BoidsHandler::get_boid(uint32_t index)
{
    if (index < 0 || index > registered_boids.size()) {
        return nullptr;
    }
    return registered_boids[index];
}

int BoidsHandler::get_boid_count() const {
    return registered_boids.size() - retired_boid_indexes.size();
}

void BoidsHandler::print_boids_debug() {
    const int boid_count = boids_buffer.unit_ids.size();

    for (int i = 0; i < boid_count; ++i) {
        godot::print_line("=== Boid #" + String::num(i) + " ===");

        godot::print_line("unit_id: " + String::num(boids_buffer.unit_ids[i]));
        godot::print_line("attribute: " + String::num(boids_buffer.attributes[i]));
        godot::print_line("active: " + String::num(boids_buffer.active[i]));

        godot::print_line("position: " + boids_buffer.positions[i]);
        godot::print_line("velocity: " + boids_buffer.velocities[i]);
        godot::print_line("attack vector: " + boids_buffer.attack_vectors[i]);
        godot::print_line("sleepy: " + boids_buffer.sleepy[i]);

        godot::print_line("view_range: " + String::num(boids_buffer.view_range[i]));
        godot::print_line("combat_range: " + String::num(boids_buffer.combat_range[i]));
        godot::print_line("collision_range: " + String::num(boids_buffer.collision_range[i]));
        godot::print_line("view_angle: " + String::num(boids_buffer.view_angle[i]));

        godot::print_line("alignment_mask: " + String::num(boids_buffer.alignment_mask[i]));
        godot::print_line("separation_mask: " + String::num(boids_buffer.separation_mask[i]));
        godot::print_line("cohesion_mask: " + String::num(boids_buffer.cohesion_mask[i]));

        godot::print_line("alignment_strength_self: " + String::num(boids_buffer.alignment_strength_self[i]));
        godot::print_line("separation_strength_self: " + String::num(boids_buffer.separation_strength_self[i]));
        godot::print_line("cohesion_strength_self: " + String::num(boids_buffer.cohesion_strength_self[i]));

        godot::print_line("alignment_strength_others: " + String::num(boids_buffer.alignment_strength_others[i]));
        godot::print_line("separation_strength_others: " + String::num(boids_buffer.separation_strength_others[i]));
        godot::print_line("cohesion_strength_others: " + String::num(boids_buffer.cohesion_strength_others[i]));

        godot::print_line("speed: " + String::num(boids_buffer.speed[i]));
        godot::print_line("max_speed: " + String::num(boids_buffer.max_speed[i]));

        godot::print_line(""); // Spacer between boids
    }
}

void BoidsHandler::_update_single_boid(uint32_t index) {
    if (index < 0 || index >= registered_boids.size() || !registered_boids[index]) {
        return;
    }

    Boid* boid = registered_boids[index];

    boids_buffer.unit_ids[index] = boid->get_unit_id();
    boids_buffer.attributes[index] = boid->get_attribute();
    boids_buffer.positions[index] = boid->get_position();
    boids_buffer.velocities[index] = boid->get_velocity();
    boids_buffer.attack_vectors[index] = boid->get_attack_vector();
    boids_buffer.sleepy[index] = boid->get_sleepy();

    boids_buffer.view_range[index] = boid->get_view_range();
    boids_buffer.combat_range[index] = boid->get_combat_range();
    boids_buffer.collision_range[index] = boid->get_collision_range();
    boids_buffer.view_angle[index] = boid->get_view_angle();
    boids_buffer.alignment_mask[index] = boid->get_alignment_mask();
    boids_buffer.separation_mask[index] = boid->get_separation_mask();
    boids_buffer.cohesion_mask[index] = boid->get_cohesion_mask();
    boids_buffer.alignment_strength_self[index] = boid->get_alignment_strength_self();
    boids_buffer.separation_strength_self[index] = boid->get_separation_strength_self();
    boids_buffer.cohesion_strength_self[index] = boid->get_cohesion_strength_self();
    boids_buffer.alignment_strength_others[index] = boid->get_alignment_strength_others();
    boids_buffer.separation_strength_others[index] = boid->get_separation_strength_others();
    boids_buffer.cohesion_strength_others[index] = boid->get_cohesion_strength_others();
    boids_buffer.speed[index] = boid->get_speed();
    boids_buffer.max_speed[index] = boid->get_max_speed();

}


void BoidsHandler::_update_positions() {
    for (int i = 0; i < registered_boids.size(); i++) {
        if (registered_boids[i]) {
            boids_buffer.positions[i] = registered_boids[i]->get_position();
        }
    }
}

void BoidsHandler::_update_velocities() {
    for (int i = 0; i < registered_boids.size(); i++) {
        if (registered_boids[i]) {
            boids_buffer.velocities[i] = registered_boids[i]->get_velocity();
        }
    }
}

void BoidsHandler::_on_attack_vector_changed(Vector2 value, uint32_t index) {
    if (registered_boids[index]) {
        boids_buffer.attack_vectors[index] = value;
    }
}

void BoidsHandler::_on_sleepy_changed(uint8_t value, uint32_t index) {
    if (registered_boids[index]) {
        boids_buffer.sleepy[index] = value;
    }
}

void BoidsHandler::_on_view_range_changed(float_t value, uint32_t index) {
    if (registered_boids[index]) {
        boids_buffer.view_range[index] = value;
    }
}

void BoidsHandler::_on_combat_range_changed(float_t value, uint32_t index) {
    if (registered_boids[index]) {
        boids_buffer.combat_range[index] = value;
    }
}

void BoidsHandler::_on_collision_range_changed(float_t value, uint32_t index) {
    if (registered_boids[index]) {
        boids_buffer.collision_range[index] = value;
    }
}

void BoidsHandler::_on_view_angle_changed(float_t value, uint32_t index) {
    if (registered_boids[index]) {
        boids_buffer.view_angle[index] = value;
    }
}

void BoidsHandler::_on_alignment_mask_changed(uint32_t value, uint32_t index) {
    if (registered_boids[index]) {
        boids_buffer.alignment_mask[index] = value;
    }
}

void BoidsHandler::_on_separation_mask_changed(uint32_t value, uint32_t index) {
    if (registered_boids[index]) {
        boids_buffer.separation_mask[index] = value;
    }
}

void BoidsHandler::_on_cohesion_mask_changed(uint32_t value, uint32_t index) {
    if (registered_boids[index]) {
        boids_buffer.cohesion_mask[index] = value;
    }
}

void BoidsHandler::_on_alignment_strength_self_changed(float_t value, uint32_t index) {
    if (registered_boids[index]) {
        boids_buffer.alignment_strength_self[index] = value;
    }
}

void BoidsHandler::_on_separation_strength_self_changed(float_t value, uint32_t index) {
    if (registered_boids[index]) {
        boids_buffer.separation_strength_self[index] = value;
    }
}

void BoidsHandler::_on_cohesion_strength_self_changed(float_t value, uint32_t index) {
    if (registered_boids[index]) {
        boids_buffer.cohesion_strength_self[index] = value;
    }
}

void BoidsHandler::_on_alignment_strength_others_changed(float_t value, uint32_t index) {
    if (registered_boids[index]) {
        boids_buffer.alignment_strength_others[index] = value;
    }
}

void BoidsHandler::_on_separation_strength_others_changed(float_t value, uint32_t index) {
    if (registered_boids[index]) {
        boids_buffer.separation_strength_others[index] = value;
    }
}

void BoidsHandler::_on_cohesion_strength_others_changed(float_t value, uint32_t index) {
    if (registered_boids[index]) {
        boids_buffer.cohesion_strength_others[index] = value;
    }
}

void BoidsHandler::_on_speed_changed(float_t value, uint32_t index) {
    if (registered_boids[index]) {
        boids_buffer.speed[index] = value;
    }
}

void BoidsHandler::_on_max_speed_changed(float_t value, uint32_t index) {
    if (registered_boids[index]) {
        boids_buffer.max_speed[index] = value;
    }
}
