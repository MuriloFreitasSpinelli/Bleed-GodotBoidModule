#ifndef UNIT_STEP_H
#define UNIT_STEP_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/array.hpp>

using namespace godot;

class UnitStep : public Node {
    GDCLASS(UnitStep, Node);

private:
    int unique_id;
    float hurt;
    float heal;
    Array inflictions;

protected:
    static void _bind_methods();

public:
    UnitStep();
    UnitStep(int unique_id, float hurt, float heal, const Array &inflictions);
    ~UnitStep();

    // Getters
    int get_unique_id() const { return unique_id; }
    float get_hurt() const { return hurt; }
    float get_heal() const { return heal; }
    Array get_inflictions() const { return inflictions; }

    // Setters
    void set_unique_id(int id) { unique_id = id; }
    void set_hurt(float h) { hurt = h; }
    void set_heal(float h) { heal = h; }
    void set_inflictions(const Array &i) { inflictions = i; }

    // Utility methods
    void join_steps(const UnitStep &other);
};

#endif //UNIT_STEP_H