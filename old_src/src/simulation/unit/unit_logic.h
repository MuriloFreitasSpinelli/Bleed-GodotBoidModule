#ifndef UNIT_LOGIC_H
#define UNIT_LOGIC_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/packed_vector2_array.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include "unit_data.h"
#include "unit_step.h"

using namespace godot;

class UnitLogic : public Resource {
    GDCLASS(UnitLogic, Resource);

protected:
    static void _bind_methods();

public:
    UnitLogic();
    ~UnitLogic();

    static Array simulate_step(const Array &nearby_units, const Ref<UnitData> &data);
    static Vector2 on_attack(const Ref<UnitStep> &unit_step);
    static Vector2 on_inflict(const Ref<UnitStep> &unit_step);
    static Vector2 on_tick(const Ref<UnitStep> &unit_step);
    static Vector2 on_heal(const Ref<UnitStep> &unit_step);
};

#endif //UNIT_LOGIC_H