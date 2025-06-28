#ifndef UNIT_BASE_DATA_H
#define UNIT_BASE_DATA_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/array.hpp>
#include "passive.h"

using namespace godot;

class UnitBaseData : public Resource {
    GDCLASS(UnitBaseData, Resource);

private:
    int unit_id;
    float base_health;
    float base_strength;
    float base_speed;
    float base_defense;
    Array passives;
    Ref<Passive> trait;

protected:
    static void _bind_methods();

public:
    UnitBaseData();
    ~UnitBaseData();

    // Getters
    int get_unit_id() const { return unit_id; }
    float get_base_health() const { return base_health; }
    float get_base_strength() const { return base_strength; }
    float get_base_speed() const { return base_speed; }
    float get_base_defense() const { return base_defense; }
    Array get_passives() const { return passives; }
    Ref<Passive> get_trait() const { return trait; }

    // Setters
    void set_unit_id(int id) { unit_id = id; }
    void set_base_health(float health) { base_health = health; }
    void set_base_strength(float strength) { base_strength = strength; }
    void set_base_speed(float speed) { base_speed = speed; }
    void set_base_defense(float defense) { base_defense = defense; }
    void set_passives(const Array &p) { passives = p; }
    void set_trait(const Ref<Passive> &t) { trait = t; }

    // Utility methods
    void add_passive(const Ref<Passive> &p) { passives.push_back(p); }
    void remove_passive(const Ref<Passive> &p) {
        int index = passives.find(p);
        if (index != -1) {
            passives.remove_at(index);
        }
    }
};

#endif //UNIT_BASE_DATA_H
