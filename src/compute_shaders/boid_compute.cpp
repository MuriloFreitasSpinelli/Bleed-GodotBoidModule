#include "boid_compute.h"

void BoidCompute::_bind_methods() {

	ClassDB::bind_method(D_METHOD("get_cell_size"), &BoidCompute::get_cell_size);
	ClassDB::bind_method(D_METHOD("set_cell_size", "value"), &BoidCompute::set_cell_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cell_size"), "set_cell_size", "get_cell_size");

	ClassDB::bind_method(D_METHOD("get_h_cells"), &BoidCompute::get_h_cells);
	ClassDB::bind_method(D_METHOD("set_h_cells", "value"), &BoidCompute::set_h_cells);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "h_cells"), "set_h_cells", "get_h_cells");

	ClassDB::bind_method(D_METHOD("get_v_cells"), &BoidCompute::get_v_cells);
	ClassDB::bind_method(D_METHOD("set_v_cells", "value"), &BoidCompute::set_v_cells);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "v_cells"), "set_v_cells", "get_v_cells");

	ClassDB::bind_method(D_METHOD("get_world_min"), &BoidCompute::get_world_min);
	ClassDB::bind_method(D_METHOD("set_world_min", "value"), &BoidCompute::set_world_min);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "world_min"), "set_world_min", "get_world_min");


	ClassDB::bind_method(D_METHOD("get_max_neighbors"), &BoidCompute::get_max_neighbors);
	ClassDB::bind_method(D_METHOD("set_max_neighbors", "value"), &BoidCompute::set_max_neighbors);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_neighbors"), "set_max_neighbors", "get_max_neighbors");

}


BoidCompute::BoidCompute()
{
	cell_size = 32;
	h_cells = 10;
	v_cells = 10;
	world_min = Vector2(0, 0);
	world_max = Vector2(h_cells * cell_size, v_cells * cell_size);
	max_neighbors = 200;

	boid_identity_buffer = rd.storage_buffer_create(1);
	boid_mode_buffer = rd.storage_buffer_create(1);
	boid_vectors_buffer = rd.storage_buffer_create(1);
	boid_ranges_buffer = rd.storage_buffer_create(1);
	boid_masks_buffer = rd.storage_buffer_create(1);
	boid_self_strength_buffer = rd.storage_buffer_create(1);
	boid_other_strength_buffer = rd.storage_buffer_create(1);
	boid_speeds_buffer = rd.storage_buffer_create(1);
}

BoidCompute::~BoidCompute() {
}


PackedByteArray BoidCompute::_int32_to_packed_byte_array(const std::vector<uint32_t>& p_array) {
	PackedByteArray bytes;
	if (p_array.empty()) { 
		return bytes; // return empty PackedByteArray
	}

	const int64_t byte_count = static_cast<int64_t>(p_array.size()) * sizeof(uint32_t);
	bytes.resize(byte_count);

	// Copy the whole buffer in one pass – PackedByteArray guarantees contiguous memory.
	std::memcpy(bytes.ptrw(), p_array.data(), byte_count);
	return bytes;
}


PackedByteArray BoidCompute::_float_to_packed_byte_array(const std::vector<float>& p_array) {
	PackedByteArray bytes;
	if (p_array.empty()) {
		return bytes;
	}

	const int64_t byte_count = static_cast<int64_t>(p_array.size()) * sizeof(float);
	bytes.resize(byte_count);
	std::memcpy(bytes.ptrw(), p_array.data(), byte_count);
	return bytes;
}


PackedByteArray BoidCompute::_vector2_to_packed_byte_array(const std::vector<Vector2>& p_array) {
	PackedByteArray bytes;
	if (p_array.empty()) {
		return bytes;
	}

	const int64_t byte_count = static_cast<int64_t>(p_array.size()) * sizeof(Vector2);
	bytes.resize(byte_count);
	std::memcpy(bytes.ptrw(), p_array.data(), byte_count);
	return bytes;
}

// TODO : If after profiling if these buffer updates and data to byte is expensive... 
// I should make the buffer hold the bytes themselves, and update the bytes directly instead of
// updating all bytes of all boids. If its still a problem, we apply a work pool so threads can write
// due updates... hopefully this stage is never reached...

//Currently a lot of writes... as well as to byte array calls.
bool BoidCompute::simulate_boids(BoidsBuffer* boids_buffer) {
	if (simulating) {
		return false
	}

	PackedByteArray boid_identity = _int32_to_packed_byte_array(boids_buffer->unit_ids);
	boid_identity.append_array(_int32_to_packed_byte_array(boids_buffer->attributes));
	rd.buffer_update(boid_identity_buffer, 0, boid_identity.size(), boid_identity);

	PackedByteArray boid_mode = _int32_to_packed_byte_array(boids_buffer->active);
	boid_mode.append_array(_int32_to_packed_byte_array(boids_buffer->sleepy));
	rd.buffer_update(boid_mode_buffer, 0, boid_mode.size(), boid_mode);

	PackedByteArray boid_vectors = _vector2_to_packed_byte_array(boids_buffer->positions);
	boid_vectors.append_array(_vector2_to_packed_byte_array(boids_buffer->velocities));
	rd.buffer_update(boid_vectors_buffer, 0, boid_vectors.size(), boid_vectors);

	PackedByteArray boid_ranges = _float_to_packed_byte_array(boids_buffer->view_range);
	boid_ranges.append_array(_float_to_packed_byte_array(boids_buffer->combat_range));
	boid_ranges.append_array(_float_to_packed_byte_array(boids_buffer->collision_range));
	boid_ranges.append_array(_float_to_packed_byte_array(boids_buffer->view_angle));
	rd.buffer_update(boid_ranges_buffer, 0, boid_ranges.size(), boid_ranges);

	PackedByteArray boid_masks = _int32_to_packed_byte_array(boids_buffer->alignment_mask);
	boid_masks.append_array(_int32_to_packed_byte_array(boids_buffer->cohesion_mask));
	boid_masks.append_array(_int32_to_packed_byte_array(boids_buffer->separation_mask));
	rd.buffer_update(boid_masks_buffer, 0, boid_masks.size(), boid_masks);

	PackedByteArray boid_self_strength = _float_to_packed_byte_array(boids_buffer->alignment_strength_self);
	boid_self_strength.append_array(_float_to_packed_byte_array(boids_buffer->cohesion_strength_self));
	boid_self_strength.append_array(_float_to_packed_byte_array(boids_buffer->separation_strength_self));
	rd.buffer_update(boid_self_strength_buffer, 0, boid_self_strength.size(), boid_self_strength);

	PackedByteArray boid_other_strength = _float_to_packed_byte_array(boids_buffer->alignment_strength_others);
	boid_other_strength.append_array(_float_to_packed_byte_array(boids_buffer->cohesion_strength_others));
	boid_other_strength.append_array(_float_to_packed_byte_array(boids_buffer->separation_strength_others));
	rd.buffer_update(boid_other_strength_buffer, 0, boid_other_strength.size(), boid_other_strength);

	PackedByteArray boid_speeds = _float_to_packed_byte_array(boids_buffer->speed);
	boid_speeds.append_array(_float_to_packed_byte_array(boids_buffer->max_speed));
	rd.buffer_update(boid_speeds_buffer, 0, boid_speeds.size(), boid_speeds);

	simulating = true;
	return true;
}

bool BoidCompute::cpu_sync() {
	if (simulating) {
		rd.sync();
		return true
	}
	return false;
}

TypedArray<PackedInt32Array> BoidCompute::get_neighbors() {
	return TypedArray<PackedInt32Array>();
}

PackedVector2Array BoidCompute::get_resulted_velocities() {
	return PackedVector2Array();
}
