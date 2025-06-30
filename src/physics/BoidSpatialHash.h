#pragma once
#pragma once

#include <vector>
#include <mutex>
#include <shared_mutex>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include 

using namespace godot;

class SpatialHash : public Node {
    GDCLASS(SpatialHash, Node)

private:
    // Grid config
    int cell_size = 32;
    int n_horizontal_cells = 10;
    int n_vertical_cells = 5;

    // Grid structure: boid IDs are stored per cell
    std::vector<std::vector<std::vector<int>>> boid_grid;
    std::vector<std::vector<std::shared_mutex>> cell_locks;

    // Lookup tables for neighbor querying
    std::vector<std::vector<Vector2i>> radius_lut;
    std::vector<std::vector<Vector2i>> radius_flattened;

protected:
    static void _bind_methods();

public:
    SpatialHash();
    void _ready();

    void add_boid(Vector2i pos, int id);
    void remove_boid(Vector2i pos, int id);
    godot::Array get_nearby_boids(Vector2i pos, int radius);
    void clear_grid();
    void resize_grid();

    Vector2i world_to_grid(Vector2 pos);
    Vector2 grid_to_world(Vector2i pos);

    // Fixed width/height calculations
    int get_world_width() const { return n_horizontal_cells * cell_size; }
    int get_world_height() const { return n_vertical_cells * cell_size; }
    Vector2 get_world_size() const { return Vector2(n_horizontal_cells * cell_size, n_vertical_cells * cell_size); }

    int get_cell_size() const { return cell_size; }
    int get_n_horizontal_cells() const { return n_horizontal_cells; }
    int get_n_vertical_cells() const { return n_vertical_cells; }

    void set_cell_size(int value);
    void set_n_horizontal_cells(int value);
    void set_n_vertical_cells(int value);

};