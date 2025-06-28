#ifndef BOID_DATA_H
#define BOID_DATA_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Data snapshot structure
struct BoidDataSnapshot {
    float world_margin;
    float world_pull_force;
    float cohesion_force;
    float separation_force;
    float alignment_force;
    float chase_force;
    float self_separation_range;
    float combat_range;
    float view_range;
    float view_angle;
    float max_speed;
    float left_border;
    float right_border;
    float top_border;
    float bottom_border;
};

class BoidData : public Resource {
    GDCLASS(BoidData, Resource)

private:
    float world_margin = 10.0f;
    float world_pull_force = 10.0f;
    float cohesion_force = 10.0f;
    float separation_force = 10.0f;
    float alignment_force = 10.0f;
    float chase_force = 10.0f;
    float self_separation_range = 20.0f;
    float combat_range = 5.0f;
    float view_range = 20.0f;
    float view_angle = 120.0f;
    float max_speed = 30.0f;

    static float left_border;
    static float right_border;
    static float top_border;
    static float bottom_border;

protected:
    static void _bind_methods();

public:
    BoidData();
    ~BoidData();

    // Static border management
    static void recalculate_borders();

    // Get data snapshot
    BoidDataSnapshot get_data() const;

    // Getters and setters for exported properties
    void set_world_margin(float p_value);
    float get_world_margin() const;

    void set_world_pull_force(float p_value);
    float get_world_pull_force() const;

    void set_cohesion_force(float p_value);
    float get_cohesion_force() const;

    void set_separation_force(float p_value);
    float get_separation_force() const;

    void set_alignment_force(float p_value);
    float get_alignment_force() const;

    void set_chase_force(float p_value);
    float get_chase_force() const;

    void set_self_separation_range(float p_value);
    float get_self_separation_range() const;

    void set_combat_range(float p_value);
    float get_combat_range() const;

    void set_view_range(float p_value);
    float get_view_range() const;

    void set_view_angle(float p_value);
    float get_view_angle() const;

    void set_max_speed(float p_value);
    float get_max_speed() const;

    // Static border getters/setters
    static void set_left_border(float p_value);
    static float get_left_border();

    static void set_right_border(float p_value);
    static float get_right_border();

    static void set_top_border(float p_value);
    static float get_top_border();

    static void set_bottom_border(float p_value);
    static float get_bottom_border();
};

#endif // BOID_DATA_H
