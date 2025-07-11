#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/vector2.hpp>

using namespace godot;

//Boid class, a custom implementation of a RigidBody2D-like for boids.
class Boid : public Node {
    GDCLASS(Boid, Node)

public:
    // Used for neighbor filtering inside the compute shaders, can be used outside for further filtering of neighbors
    // Team constants for masks
    static const int WHITE_TEAM = 1;
    static const int BLACK_TEAM = 2;
    static const int RED_TEAM = 4;

    // Layer constants for masks
    static const int ONE = 8;
    static const int TWO = 16;
    static const int THREE = 32;
    static const int FOUR = 64;
    static const int FIVE = 128;

    // Range type constants for masks
    static const int VIEW_RANGE = 256;
    static const int COMBAT_RANGE = 512;
    static const int COLLISION_RANGE = 1024;
    static const int VIEW_ANGLE = 2048;

private:
    int index; //index position in BoidsHandler
    int unit_id; //Unique id of the unique boid unit.

    //Vector2 direction; depracated, when no chase target exists its the direction it is facing else the direction to the target.
    Vector2 velocity;
    Vector2 position;
    //Boid* chase_target; depracated, to be found by the compute shader, it is any boid of any opposing team.
    Vector2 attack_vector; //vector that will be applied as an offset when approaching a chase_target

    float linear_damp; //always active, 10x more active when sleepy, as to not move as much as possible

    PackedInt32Array neighbor_indexes; //last found neighbor indexes, 1:index - 2:mask
    uint8_t sleepy; //A sleepy boid is a boid that is fighting, so no cohesion or alignment, it will try its best to no move and to not let any boid enter its space. cannot be bool due to bool not having a .data()

    // Exported properties
    int team; //Team is a single flag, acts more like an enum
    int layer; //Layer can be multiple flags, a giant may be both in the gorund and in the sky

    // Ranges
    float view_range; //Range in which boids may affect it at all
    float combat_range; //Range in which the boid fight, if any boid is found in this range boid should enter sleepy.
    float collision_range; //Collision range, this is the last line of defense againt clumping, if any boid is here it will be pushed back with great force 10x the normal separation
    float view_angle; //View angle in which the cohesion and alignment take place, boids in view range are also the only boids which may be attacked, considering the direction will always
    //point to at least one enemy boid this should not casue trouble

    // Behavior masks
    int alignment_mask; //what teams and layers affect its alignment? if not here it will be ignored.
    int separation_mask; //what teams and layers affect its separation? if not here it will be ignored.
    int cohesion_mask; //what teams and layers affect its cohesion? if not here it will be ignored.

    // Self strengths
    float alignment_strength_self; //how should it act with its own unit duplicates
    float separation_strength_self; //how should it act with its own unit duplicates
    float cohesion_strength_self; //how should it act with its own unit duplicates

    // Other strengths
    float alignment_strength_others; //how should it act with all others.
    float separation_strength_others; //how should it act with all others.
    float cohesion_strength_others; //how should it act with all others.

    // Other variables
    float speed; //speed multiplier
    float max_speed; //max speed

public:
    Boid();
    ~Boid();

    // Getters and Setters for Boid Header Variables
    // Signals for all rarely changed variables. Position and velocity have no signal, as they will be requested very often.

    // Index
    int get_index() const { return index; }
    void set_index(int value) { index = value; }

    // Unit ID
    int get_unit_id() const { return unit_id; }
    void set_unit_id(int value) { unit_id = value; }

    // Velocity
    Vector2 get_velocity() const { return velocity; }
    void set_velocity(const Vector2& value) { velocity = value; }

    // Position
    Vector2 get_position() const { return position; }
    void set_position(const Vector2& value) { position = value; }

    // Attack Vector
    Vector2 get_attack_vector() const { return attack_vector; }
    void set_attack_vector(const Vector2& value) {
        attack_vector = value;
        emit_signal("attack_vector_changed", attack_vector, index);
    }

    // Linear Damping
    float get_linear_damp() const { return linear_damp; }
    void set_linear_damp(float value) {
        linear_damp = value;
        emit_signal("linear_damp_changed", linear_damp, index);
    }

    // Neighbor Indexes
    PackedInt32Array get_neighbor_indexes() const { return neighbor_indexes; }
    void set_neighbor_indexes(const PackedInt32Array& value) { neighbor_indexes = value; }

    // Sleepy
    uint8_t get_sleepy() const { return sleepy; }
    void set_sleepy(uint8_t value) {
        sleepy = value;
        emit_signal("sleepy_changed", sleepy, index);
    }

    // Team
    int get_team() const { return team; }
    void set_team(int value) { team = value; }

    // Layer
    int get_layer() const { return layer; }
    void set_layer(int value) { layer = value; }

    // View Range
    float get_view_range() const { return view_range; }
    void set_view_range(float value) {
        view_range = value;
        emit_signal("view_range_changed", view_range, index);
    }

    // Combat Range
    float get_combat_range() const { return combat_range; }
    void set_combat_range(float value) {
        combat_range = value;
        emit_signal("combat_range_changed", combat_range, index);
    }

    // Collision Range
    float get_collision_range() const { return collision_range; }
    void set_collision_range(float value) {
        collision_range = value;
        emit_signal("collision_range_changed", collision_range, index);
    }

    // View Angle
    float get_view_angle() const { return view_angle; }
    void set_view_angle(float value) {
        view_angle = value;
        emit_signal("view_angle_changed", view_angle, index);
    }

    // Alignment Mask
    int get_alignment_mask() const { return alignment_mask; }
    void set_alignment_mask(int value) {
        alignment_mask = value;
        emit_signal("alignment_mask_changed", alignment_mask, index);
    }

    // Separation Mask
    int get_separation_mask() const { return separation_mask; }
    void set_separation_mask(int value) {
        separation_mask = value;
        emit_signal("separation_mask_changed", separation_mask, index);
    }

    // Cohesion Mask
    int get_cohesion_mask() const { return cohesion_mask; }
    void set_cohesion_mask(int value) {
        cohesion_mask = value;
        emit_signal("cohesion_mask_changed", cohesion_mask, index);
    }

    // Alignment Strength Self
    float get_alignment_strength_self() const { return alignment_strength_self; }
    void set_alignment_strength_self(float value) {
        alignment_strength_self = value;
        emit_signal("alignment_strength_self_changed", alignment_strength_self, index);
    }

    // Separation Strength Self
    float get_separation_strength_self() const { return separation_strength_self; }
    void set_separation_strength_self(float value) {
        separation_strength_self = value;
        emit_signal("separation_strength_self_changed", separation_strength_self, index);
    }

    // Cohesion Strength Self
    float get_cohesion_strength_self() const { return cohesion_strength_self; }
    void set_cohesion_strength_self(float value) {
        cohesion_strength_self = value;
        emit_signal("cohesion_strength_self_changed", cohesion_strength_self, index);
    }

    // Alignment Strength Others
    float get_alignment_strength_others() const { return alignment_strength_others; }
    void set_alignment_strength_others(float value) {
        alignment_strength_others = value;
        emit_signal("alignment_strength_others_changed", alignment_strength_others, index);
    }

    // Separation Strength Others
    float get_separation_strength_others() const { return separation_strength_others; }
    void set_separation_strength_others(float value) {
        separation_strength_others = value;
        emit_signal("separation_strength_others_changed", separation_strength_others, index);
    }

    // Cohesion Strength Others
    float get_cohesion_strength_others() const { return cohesion_strength_others; }
    void set_cohesion_strength_others(float value) {
        cohesion_strength_others = value;
        emit_signal("cohesion_strength_others_changed", cohesion_strength_others, index);
    }

    // Speed
    float get_speed() const { return speed; }
    void set_speed(float value) {
        speed = value;
        emit_signal("speed_changed", speed, index);
    }

    // Max Speed
    float get_max_speed() const { return max_speed; }
    void set_max_speed(float value) {
        max_speed = value;
        emit_signal("max_speed_changed", max_speed, index);
    }

    //Team and layer mask
    int get_attribute() const { return team | layer; };


    // Main functionality methods
    void update_boid(float delta);

protected:
    //Connect to Godot Engine
    static void _bind_methods();
};