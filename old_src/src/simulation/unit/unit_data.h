#ifndef UNIT_DATA_H
#define UNIT_DATA_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node.hpp>
#include "unit_base_data.h"

using namespace godot;

class UnitData : public Node {
    GDCLASS(UnitData, Node);

private:
    float health;
    float strength;
    float speed;
    float defense;
    int attack_radius;
    int view_radius;
    int n_attack_target;
    Ref<UnitBaseData> base_data;

protected:
    static void _bind_methods();

public:
    UnitData();
    ~UnitData();

    // Health
    void set_health(float health) { this->health = health; }
    float get_health() const { return this->health; }

    // Strength
    void set_strength(float strength) { this->strength = strength; }
    float get_strength() const { return this->strength; }

    // Speed
    void set_speed(float speed) { this->speed = speed; }
    float get_speed() const { return this->speed; }

    // Defense
    void set_defense(float defense) { this->defense = defense; }
    float get_defense() const { return this->defense; }

    // Attack radius
    void set_attack_radius(int radius) { this->attack_radius = radius; }
    int get_attack_radius() const { return this->attack_radius; }

    // View radius
    void set_view_radius(int radius) { this->view_radius = radius; }
    int get_view_radius() const { return this->view_radius; }

    // Number of attack targets
    void set_n_attack_target(int n) { this->n_attack_target = n; }
    int get_n_attack_target() const { return this->n_attack_target; }

    // Base data
    void set_base_data(const Ref<UnitBaseData> &base_data) { this->base_data = base_data; }
    Ref<UnitBaseData> get_base_data() const { return this->base_data; }
};

#endif //UNIT_DATA_H
