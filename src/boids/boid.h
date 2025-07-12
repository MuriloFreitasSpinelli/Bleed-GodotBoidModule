#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/packed_int32_array.hpp>

using namespace godot;

class Boid : public Node {
    GDCLASS(Boid, Node)

private:
    // ────────────────────────────────────────────────────────────
    //  Data members
    // ────────────────────────────────────────────────────────────
    // Identity
    uint32_t index = 0;
    uint32_t unit_id = 0;

    // Kinematics
    Vector2 velocity;
    Vector2 position;
    Vector2 attack_vector;
    real_t  linear_damp = 0.0;

    // Neighbours
    PackedInt32Array neighbor_indexes;
    uint32_t sleepy = 0; //Need to keep 4 byte alignment...

    // Attributes
    uint32_t team = WHITE_TEAM;
    uint32_t layer = ONE;

    // Ranges
    real_t view_range = 0.0;
    real_t combat_range = 0.0;
    real_t collision_range = 0.0;
    real_t view_angle = 0.0;

    // Masks
    uint32_t alignment_mask = 0;
    uint32_t separation_mask = 0;
    uint32_t cohesion_mask = 0;

    // Strengths – self
    real_t alignment_strength_self = 0.0;
    real_t separation_strength_self = 0.0;
    real_t cohesion_strength_self = 0.0;

    // Strengths – others
    real_t alignment_strength_others = 0.0;
    real_t separation_strength_others = 0.0;
    real_t cohesion_strength_others = 0.0;

    // Speed
    real_t speed = 0.0;
    real_t max_speed = 0.0;


protected:
    static void _bind_methods();

public:
    // ────────────────────────────────────────────────────────────
    //  Constants (bit‑flags)
    // ────────────────────────────────────────────────────────────
    static constexpr uint32_t WHITE_TEAM = 1;
    static constexpr uint32_t BLACK_TEAM = 2;
    static constexpr uint32_t RED_TEAM = 4;

    static constexpr uint32_t ONE = 8;
    static constexpr uint32_t TWO = 16;
    static constexpr uint32_t THREE = 32;
    static constexpr uint32_t FOUR = 64;
    static constexpr uint32_t FIVE = 128;

    static constexpr uint32_t VIEW_RANGE = 256;
    static constexpr uint32_t COMBAT_RANGE = 512;
    static constexpr uint32_t COLLISION_RANGE = 1024;
    static constexpr uint32_t VIEW_ANGLE = 2048;

    // ────────────────────────────────────────────────────────────
    //  Ctor / Dtor
    // ────────────────────────────────────────────────────────────
    Boid();
    ~Boid() override;

    // ────────────────────────────────────────────────────────────
    //  Basic identifiers
    // ────────────────────────────────────────────────────────────
    uint32_t get_index()         const { return index; }
    void     set_index(uint32_t p_value) { index = p_value; }

    uint32_t get_unit_id()       const { return unit_id; }
    void     set_unit_id(uint32_t p_value) { unit_id = p_value; }

    // ────────────────────────────────────────────────────────────
    //  Kinematics
    // ────────────────────────────────────────────────────────────
    Vector2  get_velocity()      const { return velocity; }
    void     set_velocity(const Vector2& p_value) { velocity = p_value; }

    Vector2  get_position()      const { return position; }
    void     set_position(const Vector2& p_value) { position = p_value; }

    Vector2  get_attack_vector() const { return attack_vector; }
    void     set_attack_vector(const Vector2& p_value);

    real_t   get_linear_damp()   const { return linear_damp; }
    void     set_linear_damp(real_t p_value);

    // ────────────────────────────────────────────────────────────
    //  Neighbours / state
    // ────────────────────────────────────────────────────────────
    PackedInt32Array get_neighbor_indexes() const { return neighbor_indexes; }
    void set_neighbor_indexes(const PackedInt32Array& p_value) { neighbor_indexes = p_value; }

    uint32_t get_sleepy() const { return sleepy; }
    void     set_sleepy(uint32_t p_value);

    // ────────────────────────────────────────────────────────────
    //  Attributes
    // ────────────────────────────────────────────────────────────
    uint32_t get_team()  const { return team; }
    void     set_team(uint32_t p_value);

    uint32_t get_layer() const { return layer; }
    void     set_layer(uint32_t p_value);

    // ────────────────────────────────────────────────────────────
    //  Ranges
    // ────────────────────────────────────────────────────────────
    real_t get_view_range()      const { return view_range; }
    void   set_view_range(real_t p_value);

    real_t get_combat_range()    const { return combat_range; }
    void   set_combat_range(real_t p_value);

    real_t get_collision_range() const { return collision_range; }
    void   set_collision_range(real_t p_value);

    real_t get_view_angle()      const { return view_angle; }
    void   set_view_angle(real_t p_value);

    // ────────────────────────────────────────────────────────────
    //  Behaviour masks
    // ────────────────────────────────────────────────────────────
    uint32_t get_alignment_mask()  const { return alignment_mask; }
    void     set_alignment_mask(uint32_t p_value);

    uint32_t get_separation_mask() const { return separation_mask; }
    void     set_separation_mask(uint32_t p_value);

    uint32_t get_cohesion_mask()   const { return cohesion_mask; }
    void     set_cohesion_mask(uint32_t p_value);

    // ────────────────────────────────────────────────────────────
    //  Strengths (self)
    // ────────────────────────────────────────────────────────────
    real_t get_alignment_strength_self()  const { return alignment_strength_self; }
    void   set_alignment_strength_self(real_t p_value);

    real_t get_separation_strength_self() const { return separation_strength_self; }
    void   set_separation_strength_self(real_t p_value);

    real_t get_cohesion_strength_self()   const { return cohesion_strength_self; }
    void   set_cohesion_strength_self(real_t p_value);

    // ────────────────────────────────────────────────────────────
    //  Strengths (others)
    // ────────────────────────────────────────────────────────────
    real_t get_alignment_strength_others()  const { return alignment_strength_others; }
    void   set_alignment_strength_others(real_t p_value);

    real_t get_separation_strength_others() const { return separation_strength_others; }
    void   set_separation_strength_others(real_t p_value);

    real_t get_cohesion_strength_others()   const { return cohesion_strength_others; }
    void   set_cohesion_strength_others(real_t p_value);

    // ────────────────────────────────────────────────────────────
    //  Speed
    // ────────────────────────────────────────────────────────────
    real_t get_speed()     const { return speed; }
    void   set_speed(real_t p_value);

    real_t get_max_speed() const { return max_speed; }
    void   set_max_speed(real_t p_value);

    // Combined Team | Layer (useful for shaders)
    uint32_t get_attribute() const { return team | layer; }

    // ────────────────────────────────────────────────────────────
    //  Main update
    // ────────────────────────────────────────────────────────────
    void update_boid(double p_delta);

};
