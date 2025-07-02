#include "spatial_hash.h"

using godot::Vector2;
using godot::Vector2i;

/* ---------------- Godot boiler-plate ---------------- */
void SpatialHash::_bind_methods() {
    ClassDB::bind_method(D_METHOD("add", "index", "pos"), &SpatialHash::add);
    ClassDB::bind_method(D_METHOD("remove", "index", "pos"), &SpatialHash::remove);
    ClassDB::bind_method(D_METHOD("move", "index", "old_pos", "new_pos"), &SpatialHash::move);
    /* … expose setters/getters for cell_size, horiz_cells, vert_cells if desired … */
}

SpatialHash::SpatialHash() {
    /* allocate grid & mutex vectors */
    const int cells = horiz_cells * vert_cells;
    grid.resize(cells);
    locks.resize(cells);
}

void SpatialHash::_ready() {
    /* pre-compute neighbour LUT up to, say, 5 cells radius (can be larger) */
    const int max_r = 5;
    radius_lut.resize(max_r + 1);
    for (int r = 0; r <= max_r; ++r) {
        for (int dy = -r; dy <= r; ++dy)
            for (int dx = -r; dx <= r; ++dx)
                if (dx * dx + dy * dy <= r * r)
                    radius_lut[r].push_back(Vector2i(dx, dy));
    }
}

/* ---------------- internal helpers ---------------- */
_FORCE_INLINE_ int cell_index(Vector2i c, int w, int h)
{
    if (c.x < 0 || c.y < 0 || c.x >= w || c.y >= h) return -1;
    return c.y * w + c.x;
}

/* ---------------- public API ---------------------- */
void SpatialHash::add(uint32_t idx, Vector2 pos)
{
    Vector2i c = world_to_cell(pos);
    int ci = cell_index(c, horiz_cells, vert_cells);
    if (ci < 0) return;

    std::unique_lock l(locks[ci]);
    grid[ci].push_back(idx);
}

void SpatialHash::remove(uint32_t idx, Vector2 pos)
{
    Vector2i c = world_to_cell(pos);
    int ci = cell_index(c, horiz_cells, vert_cells);
    if (ci < 0) return;

    std::unique_lock l(locks[ci]);
    auto& v = grid[ci];
    for (size_t i = 0; i < v.size(); ++i)
        if (v[i] == idx) { v[i] = v.back(); v.pop_back(); break; }
}

void SpatialHash::move(uint32_t idx, Vector2 from, Vector2 to)
{
    Vector2i cf = world_to_cell(from);
    Vector2i ct = world_to_cell(to);
    if (cf == ct) return;                // stayed in same cell

    remove(idx, from);
    add(idx, to);
}

/*  Thread-safe radius query                                             *
 *  • caller supplies scratch vector 'out' (so we don't allocate)        *
 *  • radius_px is in world units; we convert to cell radius             */
void SpatialHash::query(uint32_t /*self*/,
    Vector2 pos,
    int radius_px,
    std::vector<uint32_t>& out) const
{
    const int r_cells = (radius_px + cell_size - 1) / cell_size;
    if (r_cells >= int(radius_lut.size())) {
        ERR_PRINT("SpatialHash: query radius too big for pre-computed LUT");
        return;
    }

    Vector2i center = world_to_cell(pos);
    for (Vector2i off : radius_lut[r_cells]) {
        int ci = cell_index(center + off, horiz_cells, vert_cells);
        if (ci < 0) continue;

        std::shared_lock l(locks[ci]);            // multi-reader lock
        const auto& bucket = grid[ci];
        out.insert(out.end(), bucket.begin(), bucket.end());
    }
}
