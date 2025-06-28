//
// Created by pupul on 6/18/2025.
//
#include "spatial_hash.h"
#include <algorithm>
#include <cmath>

SpatialHash::SpatialHash() {
    // Initialize boid grid and locks
    boid_grid.resize(n_horizontal_cells);
    cell_locks.resize(n_horizontal_cells);

    for (int x = 0; x < n_horizontal_cells; ++x) {
        boid_grid[x].resize(n_vertical_cells);
        cell_locks[x] = std::vector<std::shared_mutex>(n_vertical_cells);
    }

    // Predefined offset rings (same as GDScript)
    radius_lut = {
        { Vector2i(0, 0) },
        { Vector2i(1, 0), Vector2i(-1, 0), Vector2i(0, 1), Vector2i(0, -1) },
        { Vector2i(2, 0), Vector2i(-2, 0), Vector2i(0, 2), Vector2i(0, -2),
          Vector2i(1, 1), Vector2i(-1, -1), Vector2i(1, -1), Vector2i(-1, 1) },
        { Vector2i(3, 0), Vector2i(-3, 0), Vector2i(0, 3), Vector2i(0, -3),
          Vector2i(2, 1), Vector2i(2, -1), Vector2i(-2, 1), Vector2i(-2, -1),
          Vector2i(1, 2), Vector2i(-1, 2), Vector2i(1, -2), Vector2i(-1, -2) },
        { Vector2i(4, 0), Vector2i(-4, 0), Vector2i(0, 4), Vector2i(0, -4),
          Vector2i(3, 1), Vector2i(3, -1), Vector2i(-3, 1), Vector2i(-3, -1),
          Vector2i(1, 3), Vector2i(-1, 3), Vector2i(1, -3), Vector2i(-1, -3),
          Vector2i(2, 2), Vector2i(-2, 2), Vector2i(2, -2), Vector2i(-2, -2) },
        { Vector2i(5, 0), Vector2i(-5, 0), Vector2i(0, 5), Vector2i(0, -5),
          Vector2i(4, 1), Vector2i(4, -1), Vector2i(-4, 1), Vector2i(-4, -1),
          Vector2i(1, 4), Vector2i(-1, 4), Vector2i(1, -4), Vector2i(-1, -4),
          Vector2i(3, 2), Vector2i(-3, 2), Vector2i(3, -2), Vector2i(-3, -2),
          Vector2i(2, 3), Vector2i(-2, 3), Vector2i(2, -3), Vector2i(-2, -3) }
    };

    int max_radius = radius_lut.size();
    radius_flattened.resize(max_radius);
    for (int r = 0; r < max_radius; ++r) {
        for (int i = 0; i <= r; ++i) {
            radius_flattened[r].insert(radius_flattened[r].end(),
                                       radius_lut[i].begin(),
                                       radius_lut[i].end());
        }
    }
}

void SpatialHash::_ready() {
    // Optional: put any setup logic here
}

void SpatialHash::add_boid(Vector2i pos, int id) {
    if (pos.x < 0 || pos.x >= n_horizontal_cells ||
        pos.y < 0 || pos.y >= n_vertical_cells) return;

    std::unique_lock lock(cell_locks[pos.x][pos.y]);
    auto &cell = boid_grid[pos.x][pos.y];
    if (std::find(cell.begin(), cell.end(), id) == cell.end())
        cell.push_back(id);
}

void SpatialHash::remove_boid(Vector2i pos, int id) {
    if (pos.x < 0 || pos.x >= n_horizontal_cells ||
        pos.y < 0 || pos.y >= n_vertical_cells) return;

    std::unique_lock lock(cell_locks[pos.x][pos.y]);
    auto &cell = boid_grid[pos.x][pos.y];
    cell.erase(std::remove(cell.begin(), cell.end(), id), cell.end());
}

Array SpatialHash::get_nearby_boids(Vector2i pos, int radius) {
    Array result;
    if (radius < 0 || radius >= static_cast<int>(radius_flattened.size())) return result;

    for (const auto &offset : radius_flattened[radius]) {
        Vector2i target = pos + offset;
        if (target.x >= 0 && target.x < n_horizontal_cells &&
            target.y >= 0 && target.y < n_vertical_cells) {

            std::shared_lock lock(cell_locks[target.x][target.y]);
            const auto &cell = boid_grid[target.x][target.y];
            for (int id : cell) {
                result.append(id);
            }
        }
    }
    return result;
}

void SpatialHash::clear_grid() {
    // Use unique lock for all cells to prevent access during clearing
    std::vector<std::vector<std::unique_lock<std::shared_mutex>>> locks;
    locks.resize(n_horizontal_cells);

    for (int x = 0; x < n_horizontal_cells; ++x) {
        locks[x].reserve(n_vertical_cells);
        for (int y = 0; y < n_vertical_cells; ++y) {
            locks[x].emplace_back(cell_locks[x][y]);
        }
    }

    for (int x = 0; x < n_horizontal_cells; ++x) {
        for (int y = 0; y < n_vertical_cells; ++y) {
            boid_grid[x][y].clear();
        }
    }
}

Vector2i SpatialHash::world_to_grid(Vector2 pos) {
    int gx = std::clamp(static_cast<int>(std::floor(pos.x / cell_size)), 0, n_horizontal_cells - 1);
    int gy = std::clamp(static_cast<int>(std::floor(pos.y / cell_size)), 0, n_vertical_cells - 1);
    return Vector2i(gx, gy);
}

Vector2 SpatialHash::grid_to_world(Vector2i pos) {
    float wx = pos.x * cell_size + 0.5f * cell_size;
    float wy = pos.y * cell_size + 0.5f * cell_size;
    return Vector2(wx, wy);
}

void SpatialHash::resize_grid() {
    // Lock all cells during resize to prevent race conditions
    std::vector<std::vector<std::unique_lock<std::shared_mutex>>> old_locks;
    old_locks.resize(boid_grid.size());

    for (size_t x = 0; x < boid_grid.size(); ++x) {
        old_locks[x].reserve(boid_grid[x].size());
        for (size_t y = 0; y < boid_grid[x].size(); ++y) {
            old_locks[x].emplace_back(cell_locks[x][y]);
        }
    }

    // Now safely resize
    boid_grid.clear();
    cell_locks.clear();

    boid_grid.resize(n_horizontal_cells);
    cell_locks.resize(n_horizontal_cells);

    for (int x = 0; x < n_horizontal_cells; ++x) {
        boid_grid[x].resize(n_vertical_cells);
        cell_locks[x] = std::vector<std::shared_mutex>(n_vertical_cells);
    }

}

void SpatialHash::set_cell_size(int value) {
    if (value != cell_size && value > 0) {
        cell_size = value;
        resize_grid(); // Rebuild data structures
    }
}

void SpatialHash::set_n_horizontal_cells(int value) {
    if (value != n_horizontal_cells && value > 0) {
        n_horizontal_cells = value;
        resize_grid(); // Rebuild data structures
    }
}

void SpatialHash::set_n_vertical_cells(int value) {
    if (value != n_vertical_cells && value > 0) {
        n_vertical_cells = value;
        resize_grid(); // Rebuild data structures
    }
}



void SpatialHash::_bind_methods() {
    ClassDB::bind_method(D_METHOD("add_boid", "pos", "id"), &SpatialHash::add_boid);
    ClassDB::bind_method(D_METHOD("remove_boid", "pos", "id"), &SpatialHash::remove_boid);
    ClassDB::bind_method(D_METHOD("get_nearby_boids", "pos", "radius"), &SpatialHash::get_nearby_boids);
    ClassDB::bind_method(D_METHOD("clear_grid"), &SpatialHash::clear_grid);
    ClassDB::bind_method(D_METHOD("world_to_grid", "pos"), &SpatialHash::world_to_grid);
    ClassDB::bind_method(D_METHOD("grid_to_world", "pos"), &SpatialHash::grid_to_world);
    ClassDB::bind_method(D_METHOD("get_world_width"), &SpatialHash::get_world_width);
    ClassDB::bind_method(D_METHOD("get_world_height"), &SpatialHash::get_world_height);
    ClassDB::bind_method(D_METHOD("get_world_size"), &SpatialHash::get_world_size);

    // Inspector properties
    ClassDB::bind_method(D_METHOD("set_cell_size", "value"), &SpatialHash::set_cell_size);
    ClassDB::bind_method(D_METHOD("get_cell_size"), &SpatialHash::get_cell_size);
    ClassDB::add_property("SpatialHash", PropertyInfo(Variant::INT, "cell_size"), "set_cell_size", "get_cell_size");

    ClassDB::bind_method(D_METHOD("set_n_horizontal_cells", "value"), &SpatialHash::set_n_horizontal_cells);
    ClassDB::bind_method(D_METHOD("get_n_horizontal_cells"), &SpatialHash::get_n_horizontal_cells);
    ClassDB::add_property("SpatialHash", PropertyInfo(Variant::INT, "n_horizontal_cells"), "set_n_horizontal_cells", "get_n_horizontal_cells");

    ClassDB::bind_method(D_METHOD("set_n_vertical_cells", "value"), &SpatialHash::set_n_vertical_cells);
    ClassDB::bind_method(D_METHOD("get_n_vertical_cells"), &SpatialHash::get_n_vertical_cells);
    ClassDB::add_property("SpatialHash", PropertyInfo(Variant::INT, "n_vertical_cells"), "set_n_vertical_cells", "get_n_vertical_cells");
}
