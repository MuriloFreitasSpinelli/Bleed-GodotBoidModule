#pragma once
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node.hpp>

#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/rd_shader_file.hpp>
#include <godot_cpp/classes/rd_shader_spirv.hpp>

#include "../boids/boids_handler.h"

using namespace godot;

class BoidCompute : public Node {
	GDCLASS(BoidCompute, Node)

private:
	Ref<RenderingDevice> rd;

	//Shaders
	RID count_sort_hash_shader;
	RID count_sort_neighbor_query_shader;
	RID boid_behaviour_shader;
	RID dummy_compute_shader;

	//Boid Data
	RID boid_identity_buffer;
	RID boid_mode_buffer;
	RID boid_vectors_buffer;
	RID boid_ranges_buffer;
	RID boid_masks_buffer;
	RID boid_self_strength_buffer;
	RID boid_other_strength_buffer;
	RID boid_speeds_buffer;

	//Spatial Hashing
	RID boid_per_cell_count_buffer;
	RID cell_offsets_buffer;
	RID spatial_map_buffer;

	//Neighhbor Query
	RID neighbor_count_buffer;
	RID neighbors_buffer;

	//Boid behaviour
	RID resulted_velocity_buffer;

	//Simulation Data
	bool simulating;

	int cell_size;
	int h_cells;
	int v_cells;
	Vector2 world_min;
	Vector2 world_max;
	int max_neighbors;

	PackedByteArray _int32_to_packed_byte_array(std::vector<uint32_t> array);
	PackedByteArray _vector2_to_packed_byte_array(std::vector<Vector2> array);
	PackedByteArray _float_to_packed_byte_array(std::vector<float_t> array);

protected:
	static void _bind_methods();
public:
	BoidCompute();
	~BoidCompute();

	bool simulate_boids(BoidsBuffer* boids_buffer);
	bool cpu_sync();
	TypedArray<PackedInt32Array> get_neighbors();
	PackedVector2Array get_resulted_velocities();

	int get_cell_size() { return cell_size; }
	void set_cell_size(int value) { 
		cell_size = value;
		recalculate_world_max();
	}

	int get_h_cells() { return h_cells; }
	void set_h_cells(int value) { 
		h_cells = value; 
		recalculate_world_max();
	}

	int get_v_cells() { return v_cells; }
	void set_v_cells(int value) { 
		v_cells = value; 
		recalculate_world_max();
	}

	Vector2 get_world_min() { return world_min; }
	void set_world_min(Vector2 value) { world_min = value; }

	Vector2 get_world_max() { return world_max; }
	void recalculate_world_max() { world_max = Vector2(h_cells * cell_size, v_cells * cell_size); }

	int get_max_neighbors() { return max_neighbors; }
	void set_max_neighbors(int value) { max_neighbors = value; }
};