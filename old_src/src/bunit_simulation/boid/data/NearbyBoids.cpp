#include "NearbyBoids.h"

using namespace godot;

NearbyBoids::NearbyBoids()
    : in_range(Array()), in_view(Array()), in_combat(Array()) {
}

NearbyBoids::NearbyBoids(Array p_in_range, Array p_in_view, Array p_in_combat)
    : in_range(p_in_range), in_view(p_in_view), in_combat(p_in_combat) {
}

void NearbyBoids::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_in_range"), &NearbyBoids::get_in_range);
    ClassDB::bind_method(D_METHOD("get_in_view"), &NearbyBoids::get_in_view);
    ClassDB::bind_method(D_METHOD("get_in_combat"), &NearbyBoids::get_in_combat);
}
