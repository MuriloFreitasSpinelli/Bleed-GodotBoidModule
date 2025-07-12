#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <cstring>
#include <memory>
#include <unordered_map>
#include <shared_mutex>
#include <vector>
#include <variant>
#include "boid.h"

using namespace godot;

enum BufferTaskType {
    UNIT_ID,
    ATTRIBUTE,
    ACTIVE_FLAG,
    SLEEPY_FLAG,
    POSITION,
    VELOCITY,
    ATTACK_VECTOR,
    VIEW_RANGE,
    COMBAT_RANGE,
    COLLISION_RANGE,
    VIEW_ANGLE,
    ALIGNMENT_MASK,
    SEPARATION_MASK,
    COHESION_MASK,
    ALIGNMENT_SELF,
    SEPARATION_SELF,
    COHESION_SELF,
    ALIGNMENT_OTHER,
    SEPARATION_OTHER,
    COHESION_OTHER,
    SPEED,
    MAX_SPEED,
};

union BufferTaskValue {
    float_t float_val;
    uint32_t uint_val;
    Vector2 vector2_val;

    BufferTaskValue() : float_val(0.0f) {}
    BufferTaskValue(float_t val) : float_val(val) {}
    BufferTaskValue(uint32_t val) : uint_val(val) {}
    BufferTaskValue(Vector2 val) : vector2_val(val) {}
};

struct BufferTask {
    BufferTaskType buffer_task_type;
    uint32_t index;
    BufferTaskValue value;
    std::atomic<bool> processed{ false };

    BufferTask(BufferTaskType type, uint32_t idx, float_t val)
        : buffer_task_type(type), index(idx), value(val) {
    }

    BufferTask(BufferTaskType type, uint32_t idx, uint32_t val)
        : buffer_task_type(type), index(idx), value(val) {
    }

    BufferTask(BufferTaskType type, uint32_t idx, Vector2 val)
        : buffer_task_type(type), index(idx), value(val) {
    }
};

struct BufferTaskKey {
    BufferTaskType buffer_task_type;
    uint32_t index;

    bool operator==(const BufferTaskKey& other) const {
        return buffer_task_type == other.buffer_task_type && index == other.index;
    }
};

struct BufferTaskHash {
    std::size_t operator()(const BufferTaskKey& key) const {
        return std::hash<uint32_t>{}(static_cast<uint32_t>(key.buffer_task_type)) ^
            (std::hash<uint32_t>{}(key.index) << 1);
    }
};

using BufferTaskPtr = std::shared_ptr<BufferTask>;

class BoidsDataBuffer : public Node {
	GDCLASS(BoidsDataBuffer, Node)

private:
    PackedByteArray unit_ids;
    PackedByteArray attributes;

    PackedByteArray active_flags;
    PackedByteArray sleepy_flags;

    PackedByteArray positions;
    PackedByteArray velocities;
    PackedByteArray attack_vectors;

    PackedByteArray view_ranges;
    PackedByteArray combat_ranges;
    PackedByteArray collision_ranges;
    PackedByteArray view_angles;

    PackedByteArray alignment_masks;
    PackedByteArray separation_masks;
    PackedByteArray cohesion_masks;

    PackedByteArray alignment_strengths_self;
    PackedByteArray separation_strengths_self;
    PackedByteArray cohesion_strengths_self;

    PackedByteArray alignment_strengths_others;
    PackedByteArray separation_strengths_others;
    PackedByteArray cohesion_strengths_others;

    PackedByteArray speeds;
    PackedByteArray max_speeds;

    int buffer_size;

    std::vector<Boid*> boids;
    std::shared_mutex boids_mutex;

    std::unordered_map<BufferTaskKey, BufferTaskPtr, BufferTaskHash> queued_tasks;
    std::mutex tasks_mutex;

    PackedByteArray& get_array_for_task_type(BufferTaskType type);
    size_t get_data_size(BufferTaskType type);
    void increment_buffer_size_to(int new_size);
    void queue_boid_data(Boid* boid, uint32_t target_index);
    bool is_active(uint32_t index) const;
    void set_active_flag(uint32_t index, bool value);
    void apply_task_to_buffer(BufferTaskPtr task); 

protected:
    static void _bind_methods();

public:
    BoidsDataBuffer();
    ~BoidsDataBuffer();

    void add_boid(Boid* boid);
    void remove_boid(Boid* boid);

    void queue_buffer_task(BufferTaskKey key, BufferTaskPtr task);  // Add parameters
    void process_all_buffer_tasks();
    void process_kinematics();

    // All three overloads for write_to_buffer_direct
    void write_to_buffer_direct(BufferTaskType type, uint32_t index, const Vector2& value);
    void write_to_buffer_direct(BufferTaskType type, uint32_t index, float_t value);
    void write_to_buffer_direct(BufferTaskType type, uint32_t index, uint32_t value);
    
    // --- Getters -------------------------------------------------
    const PackedByteArray& get_unit_ids()                 const { return unit_ids; }
    const PackedByteArray& get_attributes()               const { return attributes; }

    const PackedByteArray& get_active_flags()             const { return active_flags; }
    const PackedByteArray& get_sleepy_flags()             const { return sleepy_flags; }

    const PackedByteArray& get_positions()                const { return positions; }
    const PackedByteArray& get_velocities()               const { return velocities; }
    const PackedByteArray& get_attack_vectors()           const { return attack_vectors; }

    const PackedByteArray& get_view_ranges()              const { return view_ranges; }
    const PackedByteArray& get_combat_ranges()            const { return combat_ranges; }
    const PackedByteArray& get_collision_ranges()         const { return collision_ranges; }
    const PackedByteArray& get_view_angles()              const { return view_angles; }

    const PackedByteArray& get_alignment_masks()          const { return alignment_masks; }
    const PackedByteArray& get_separation_masks()         const { return separation_masks; }
    const PackedByteArray& get_cohesion_masks()           const { return cohesion_masks; }

    const PackedByteArray& get_alignment_strengths_self() const { return alignment_strengths_self; }
    const PackedByteArray& get_separation_strengths_self()const { return separation_strengths_self; }
    const PackedByteArray& get_cohesion_strengths_self()  const { return cohesion_strengths_self; }

    const PackedByteArray& get_alignment_strengths_others() const { return alignment_strengths_others; }
    const PackedByteArray& get_separation_strengths_others() const { return separation_strengths_others; }
    const PackedByteArray& get_cohesion_strengths_others()   const { return cohesion_strengths_others; }

    const PackedByteArray& get_speeds()                   const { return speeds; }
    const PackedByteArray& get_max_speeds()               const { return max_speeds; }

};
