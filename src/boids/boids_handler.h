#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <cstring>
#include "boid.h"

using namespace godot;
// TODO : If after profiling if these buffer updates and data to byte is expensive... 
// I should make the buffer hold the bytes themselves, and update the bytes directly instead of
// updating all bytes of all boids. If its still a problem, we apply a work pool so threads can write
// due updates... hopefully this stage is never reached...

struct BoidsBuffer {
    //Update all byte arrays
    bool new_boids;

    //Boid Identity
    std::vector<uint32_t> unit_ids;
    std::vector<uint32_t> attributes;

    //Boid Modes
    std::vector<uint32_t> active;
    std::vector<uint32_t> sleepy;
    bool mode_changed; //Update mode bytes

    //Boid Vectors
    std::vector<Vector2> positions;
    std::vector<Vector2> velocities;
    std::vector<Vector2> attack_vectors;

    //Boid Ranges
    std::vector<float_t> view_range;
    std::vector<float_t> combat_range;
    std::vector<float_t> collision_range;
    std::vector<float_t> view_angle;
    bool ranges_changed; //Update range bytes

    //Boid Masks
    std::vector<uint32_t> alignment_mask;
    std::vector<uint32_t> separation_mask;
    std::vector<uint32_t> cohesion_mask;
    bool masks_changes; //Update mask bytes

    //Boid self strengths
    std::vector<float_t> alignment_strength_self;
    std::vector<float_t> separation_strength_self;
    std::vector<float_t> cohesion_strength_self;
    bool self_strength_changed; //Update self_strength bytes

    //Boid other strengths
    std::vector<float_t> alignment_strength_others;
    std::vector<float_t> separation_strength_others;
    std::vector<float_t> cohesion_strength_others;
    bool other_strength_changed; //Update Other strength bytes

    //Boid speeds
    std::vector<float_t> speed;
    std::vector<float_t> max_speed;
    bool speeds_changed; //Update speeds bytes
};

class BoidsHandler : public Node {
    GDCLASS(BoidsHandler, Node)

private:
    //Everything is indexed by the index in registered boids...
    std::vector<Boid*> registered_boids;
    std::vector<uint32_t> retired_boid_indexes;
    BoidsBuffer boids_buffer;

    //Runs on every buffer request
    void _update_positions();
    void _update_velocities();

    //Runs when new boid is added
    void _update_single_boid(uint32_t index);

    //Runs only when boid signal is called
    void _on_attack_vector_changed(Vector2 value, uint32_t index);
    void _on_sleepy_changed(uint8_t value, uint32_t index);
    void _on_view_range_changed(float_t value, uint32_t index);
    void _on_combat_range_changed(float_t value, uint32_t index);
    void _on_collision_range_changed(float_t value, uint32_t index);
    void _on_view_angle_changed(float_t value, uint32_t index);
    void _on_alignment_mask_changed(uint32_t value, uint32_t index);
    void _on_separation_mask_changed(uint32_t value, uint32_t index);
    void _on_cohesion_mask_changed(uint32_t value, uint32_t index);
    void _on_alignment_strength_self_changed(float_t value, uint32_t index);
    void _on_separation_strength_self_changed(float_t value, uint32_t index);
    void _on_cohesion_strength_self_changed(float_t value, uint32_t index);
    void _on_alignment_strength_others_changed(float_t value, uint32_t index);
    void _on_separation_strength_others_changed(float_t value, uint32_t index);
    void _on_cohesion_strength_others_changed(float_t value, uint32_t index);
    void _on_speed_changed(float_t value, uint32_t index);
    void _on_max_speed_changed(float_t value, uint32_t index);

protected:
    static void _bind_methods();

public:
    BoidsHandler();
    ~BoidsHandler();

    //Boid management
    void register_boid(Boid* boid);
    void retire_boid(uint32_t boid_index);
    void batch_register_boids(TypedArray<Boid> boids);
    void batch_retire_boids(TypedArray<uint32_t> boid_indexes);

    //Buffer functionality
    void update_buffer();

    //Boid functionality - Godot variable types
    void update_boids(double delta);
    void update_boids_velocity(PackedVector2Array new_velocities);
    void update_boids_neighbors(TypedArray<PackedInt32Array> new_neighbors);

    //Ultility
    Boid* get_boid(uint32_t index);
    uint32_t get_boid_count() const;

    //Debug
    void print_boids_debug();

};