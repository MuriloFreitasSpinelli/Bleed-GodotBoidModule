#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/resource.hpp>

using namespace godot;

//One Worker thread per row so no mutex needed
struct BoidsSoA {
    //Constant updates
    std::vector<float> pos_x, pos_y;
    std::vector<float> vel_x, vel_y;

    //Rarely updated
    std::vector<uint64_t> attr_key;
    std::vector<float> view_range;
    std::vector<float> view_angle;
    std::vector<float> max_speed;

    size_t size() const { return pos_x.size(); }
};

class BoidManager : public Node {
	GDCLASS(X, Node)

private:
    BoidsSoA read_buf, write_buf;             //Both are ordered by index, Swap at the end of a frame
    std::vector<BoidBody2D> registered_boids; //BoidBody2D knows its index,
    std::vector<bool> sleepy_boids;           //Boids that have not effectively moved for some time, lower physics tick rate

    std::vector<uint32_t> retired_indexes;    //Index of removed boids, if a new one is added use retired indexes if any


protected:
    static void _bind_methods();

    void gather_neighbors(uint_32_t self_index, NeighborMap& out_map); //Runs on Thread
    Vector2 simulate_next_step(NeighborMap& nb_map, uint16_t self_index); //Runs on Thread returns new_velocity
    
    void sync_from_scene(); //Update Soas runs on main
    void apply_to_scene(); //Update nodes runs on main


public:
    void _ready() override;
    void _physics_process(double delta) override; //Runs on Main alocated threads and calls thread methods

    uint32_t add_boid(BoidBody2D* node);
    void remove_boid(uint32_t unique_id);

    void full_soa_sync_for(uint32_t self_index);
};
