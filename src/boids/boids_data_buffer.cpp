#include "boids_data_buffer.h"
#include <godot_cpp/core/class_db.hpp>
#include <thread>
#include <future>
#include <algorithm>

void BoidsDataBuffer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("add_boid", "boid"), &BoidsDataBuffer::add_boid);
    ClassDB::bind_method(D_METHOD("remove_boid", "boid"), &BoidsDataBuffer::remove_boid);
    ClassDB::bind_method(D_METHOD("process_all_buffer_tasks"), &BoidsDataBuffer::process_all_buffer_tasks);
    ClassDB::bind_method(D_METHOD("process_kinematics"), &BoidsDataBuffer::process_kinematics);

    // Getters
    ClassDB::bind_method(D_METHOD("get_unit_ids"), &BoidsDataBuffer::get_unit_ids);
    ClassDB::bind_method(D_METHOD("get_attributes"), &BoidsDataBuffer::get_attributes);
    ClassDB::bind_method(D_METHOD("get_active_flags"), &BoidsDataBuffer::get_active_flags);
    ClassDB::bind_method(D_METHOD("get_sleepy_flags"), &BoidsDataBuffer::get_sleepy_flags);
    ClassDB::bind_method(D_METHOD("get_positions"), &BoidsDataBuffer::get_positions);
    ClassDB::bind_method(D_METHOD("get_velocities"), &BoidsDataBuffer::get_velocities);
    ClassDB::bind_method(D_METHOD("get_attack_vectors"), &BoidsDataBuffer::get_attack_vectors);
    ClassDB::bind_method(D_METHOD("get_view_ranges"), &BoidsDataBuffer::get_view_ranges);
    ClassDB::bind_method(D_METHOD("get_combat_ranges"), &BoidsDataBuffer::get_combat_ranges);
    ClassDB::bind_method(D_METHOD("get_collision_ranges"), &BoidsDataBuffer::get_collision_ranges);
    ClassDB::bind_method(D_METHOD("get_view_angles"), &BoidsDataBuffer::get_view_angles);
    ClassDB::bind_method(D_METHOD("get_alignment_masks"), &BoidsDataBuffer::get_alignment_masks);
    ClassDB::bind_method(D_METHOD("get_separation_masks"), &BoidsDataBuffer::get_separation_masks);
    ClassDB::bind_method(D_METHOD("get_cohesion_masks"), &BoidsDataBuffer::get_cohesion_masks);
    ClassDB::bind_method(D_METHOD("get_alignment_strengths_self"), &BoidsDataBuffer::get_alignment_strengths_self);
    ClassDB::bind_method(D_METHOD("get_separation_strengths_self"), &BoidsDataBuffer::get_separation_strengths_self);
    ClassDB::bind_method(D_METHOD("get_cohesion_strengths_self"), &BoidsDataBuffer::get_cohesion_strengths_self);
    ClassDB::bind_method(D_METHOD("get_alignment_strengths_others"), &BoidsDataBuffer::get_alignment_strengths_others);
    ClassDB::bind_method(D_METHOD("get_separation_strengths_others"), &BoidsDataBuffer::get_separation_strengths_others);
    ClassDB::bind_method(D_METHOD("get_cohesion_strengths_others"), &BoidsDataBuffer::get_cohesion_strengths_others);
    ClassDB::bind_method(D_METHOD("get_speeds"), &BoidsDataBuffer::get_speeds);
    ClassDB::bind_method(D_METHOD("get_max_speeds"), &BoidsDataBuffer::get_max_speeds);
}

BoidsDataBuffer::BoidsDataBuffer() : buffer_size(0) {
}

BoidsDataBuffer::~BoidsDataBuffer() {
}

void BoidsDataBuffer::add_boid(Boid* boid) {
    if (!boid) return;

    uint32_t target_index = boid->get_index();

    // Resize buffer if needed
    if (target_index >= buffer_size) {
        increment_buffer_size_to(target_index + 1);
    }

    {
        std::unique_lock<std::shared_mutex> lock(boids_mutex);
        // Ensure boids vector is large enough
        if (target_index >= boids.size()) {
            boids.resize(target_index + 1, nullptr);
        }
        boids[target_index] = boid;
    }

    // Activate the slot
    set_active_flag(target_index, true);

    // Queue initial data for this boid
    queue_boid_data(boid, target_index);
}

void BoidsDataBuffer::remove_boid(Boid* boid) {
    if (!boid) return;

    uint32_t index = boid->get_index();

    // Deactivate first to stop compute shader processing
    set_active_flag(index, false);

    {
        std::unique_lock<std::shared_mutex> lock(boids_mutex);
        if (index < boids.size()) {
            boids[index] = nullptr;
        }
    }

    // Clear any pending tasks for this index
    {
        std::lock_guard<std::mutex> lock(tasks_mutex);
        auto it = queued_tasks.begin();
        while (it != queued_tasks.end()) {
            if (it->first.index == index) {
                it = queued_tasks.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}

void BoidsDataBuffer::queue_buffer_task(BufferTaskKey key, BufferTaskPtr task) {
    std::lock_guard<std::mutex> lock(tasks_mutex);
    // Replace existing task if one exists for this key
    queued_tasks[key] = task;
}

void BoidsDataBuffer::process_all_buffer_tasks() {
    std::vector<BufferTaskPtr> tasks_to_process;

    // Extract tasks while holding the lock
    {
        std::lock_guard<std::mutex> lock(tasks_mutex);
        tasks_to_process.reserve(queued_tasks.size());

        for (auto& [key, task] : queued_tasks) {
            if (is_active(key.index) && !task->processed.load()) {
                tasks_to_process.push_back(task);
            }
        }
    }

    // Process tasks in parallel
    if (!tasks_to_process.empty()) {
        const size_t num_threads = std::thread::hardware_concurrency();
        const size_t tasks_per_thread = (tasks_to_process.size() + num_threads - 1) / num_threads;

        std::vector<std::future<void>> futures;

        for (size_t t = 0; t < num_threads; ++t) {
            size_t start = t * tasks_per_thread;
            size_t end = std::min(start + tasks_per_thread, tasks_to_process.size());

            if (start < end) {
                futures.push_back(std::async(std::launch::async, [this, &tasks_to_process, start, end]() {
                    for (size_t i = start; i < end; ++i) {
                        auto& task = tasks_to_process[i];
                        apply_task_to_buffer(task);
                        task->processed.store(true);
                    }
                    }));
            }
        }

        // Wait for all threads to complete
        for (auto& future : futures) {
            future.wait();
        }
    }

    // Clear processed tasks
    {
        std::lock_guard<std::mutex> lock(tasks_mutex);
        auto it = queued_tasks.begin();
        while (it != queued_tasks.end()) {
            if (it->second->processed.load()) {
                it = queued_tasks.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}

void BoidsDataBuffer::process_kinematics() {
    std::shared_lock<std::shared_mutex> lock(boids_mutex);

    const size_t num_threads = 2;
    const size_t boids_per_thread = (boids.size() + num_threads - 1) / num_threads;

    std::vector<std::future<void>> futures;

    for (size_t t = 0; t < num_threads; ++t) {
        size_t start = t * boids_per_thread;
        size_t end = std::min(start + boids_per_thread, boids.size());

        if (start < end) {
            futures.push_back(std::async(std::launch::async, [this, start, end]() {
                for (size_t i = start; i < end; ++i) {
                    if (boids[i] && is_active(i)) {
                        Vector2 pos = boids[i]->get_position();
                        Vector2 vel = boids[i]->get_velocity();
                        write_to_buffer_direct(POSITION, i, pos);
                        write_to_buffer_direct(VELOCITY, i, vel);
                    }
                }
                }));
        }
    }

    // Wait for all threads to complete
    for (auto& future : futures) {
        future.wait();
    }
}


// Private helper methods

PackedByteArray& BoidsDataBuffer::get_array_for_task_type(BufferTaskType type) {
    switch (type) {
    case UNIT_ID: return unit_ids;
    case ATTRIBUTE: return attributes;
    case ACTIVE_FLAG: return active_flags;
    case SLEEPY_FLAG: return sleepy_flags;
    case POSITION: return positions;
    case VELOCITY: return velocities;
    case ATTACK_VECTOR: return attack_vectors;
    case VIEW_RANGE: return view_ranges;
    case COMBAT_RANGE: return combat_ranges;
    case COLLISION_RANGE: return collision_ranges;
    case VIEW_ANGLE: return view_angles;
    case ALIGNMENT_MASK: return alignment_masks;
    case SEPARATION_MASK: return separation_masks;
    case COHESION_MASK: return cohesion_masks;
    case ALIGNMENT_SELF: return alignment_strengths_self;
    case SEPARATION_SELF: return separation_strengths_self;
    case COHESION_SELF: return cohesion_strengths_self;
    case ALIGNMENT_OTHER: return alignment_strengths_others;
    case SEPARATION_OTHER: return separation_strengths_others;
    case COHESION_OTHER: return cohesion_strengths_others;
    case SPEED: return speeds;
    case MAX_SPEED: return max_speeds;
    default: return unit_ids; // fallback
    }
}

size_t BoidsDataBuffer::get_data_size(BufferTaskType type) {
    switch (type) {
    case UNIT_ID:
    case ATTRIBUTE:
    case ALIGNMENT_MASK:
    case SEPARATION_MASK:
    case COHESION_MASK:
    case ACTIVE_FLAG:
    case SLEEPY_FLAG:
        return sizeof(uint32_t);

    case POSITION:
    case VELOCITY:
    case ATTACK_VECTOR:
        return sizeof(Vector2);

    case VIEW_RANGE:
    case COMBAT_RANGE:
    case COLLISION_RANGE:
    case VIEW_ANGLE:
    case ALIGNMENT_SELF:
    case SEPARATION_SELF:
    case COHESION_SELF:
    case ALIGNMENT_OTHER:
    case SEPARATION_OTHER:
    case COHESION_OTHER:
    case SPEED:
    case MAX_SPEED:
        return sizeof(float_t);

    default:
        return sizeof(float_t);
    }
}

void BoidsDataBuffer::increment_buffer_size_to(int new_size) {
    if (new_size <= buffer_size) return;

    int old_size = buffer_size;
    buffer_size = new_size;

    // Resize all arrays
    unit_ids.resize(buffer_size * sizeof(uint32_t));
    attributes.resize(buffer_size * sizeof(uint32_t));
    active_flags.resize(buffer_size * sizeof(uint32_t));
    sleepy_flags.resize(buffer_size * sizeof(uint32_t));
    positions.resize(buffer_size * sizeof(Vector2));
    velocities.resize(buffer_size * sizeof(Vector2));
    attack_vectors.resize(buffer_size * sizeof(Vector2));
    view_ranges.resize(buffer_size * sizeof(float_t));
    combat_ranges.resize(buffer_size * sizeof(float_t));
    collision_ranges.resize(buffer_size * sizeof(float_t));
    view_angles.resize(buffer_size * sizeof(float_t));
    alignment_masks.resize(buffer_size * sizeof(uint32_t));
    separation_masks.resize(buffer_size * sizeof(uint32_t));
    cohesion_masks.resize(buffer_size * sizeof(uint32_t));
    alignment_strengths_self.resize(buffer_size * sizeof(float_t));
    separation_strengths_self.resize(buffer_size * sizeof(float_t));
    cohesion_strengths_self.resize(buffer_size * sizeof(float_t));
    alignment_strengths_others.resize(buffer_size * sizeof(float_t));
    separation_strengths_others.resize(buffer_size * sizeof(float_t));
    cohesion_strengths_others.resize(buffer_size * sizeof(float_t));
    speeds.resize(buffer_size * sizeof(float_t));
    max_speeds.resize(buffer_size * sizeof(float_t));

    // Initialize new slots to zero
    if (old_size < buffer_size) {
        size_t bytes_to_clear = (buffer_size - old_size);

        memset(unit_ids.ptrw() + old_size * sizeof(uint32_t), 0, bytes_to_clear * sizeof(uint32_t));
        memset(attributes.ptrw() + old_size * sizeof(uint32_t), 0, bytes_to_clear * sizeof(uint32_t));
        memset(active_flags.ptrw() + old_size * sizeof(uint32_t), 0, bytes_to_clear * sizeof(uint32_t));
        memset(sleepy_flags.ptrw() + old_size * sizeof(uint32_t), 0, bytes_to_clear * sizeof(uint32_t));
        memset(positions.ptrw() + old_size * sizeof(Vector2), 0, bytes_to_clear * sizeof(Vector2));
        memset(velocities.ptrw() + old_size * sizeof(Vector2), 0, bytes_to_clear * sizeof(Vector2));
        memset(attack_vectors.ptrw() + old_size * sizeof(Vector2), 0, bytes_to_clear * sizeof(Vector2));
        memset(view_ranges.ptrw() + old_size * sizeof(float_t), 0, bytes_to_clear * sizeof(float_t));
        memset(combat_ranges.ptrw() + old_size * sizeof(float_t), 0, bytes_to_clear * sizeof(float_t));
        memset(collision_ranges.ptrw() + old_size * sizeof(float_t), 0, bytes_to_clear * sizeof(float_t));
        memset(view_angles.ptrw() + old_size * sizeof(float_t), 0, bytes_to_clear * sizeof(float_t));
        memset(alignment_masks.ptrw() + old_size * sizeof(uint32_t), 0, bytes_to_clear * sizeof(uint32_t));
        memset(separation_masks.ptrw() + old_size * sizeof(uint32_t), 0, bytes_to_clear * sizeof(uint32_t));
        memset(cohesion_masks.ptrw() + old_size * sizeof(uint32_t), 0, bytes_to_clear * sizeof(uint32_t));
        memset(alignment_strengths_self.ptrw() + old_size * sizeof(float_t), 0, bytes_to_clear * sizeof(float_t));
        memset(separation_strengths_self.ptrw() + old_size * sizeof(float_t), 0, bytes_to_clear * sizeof(float_t));
        memset(cohesion_strengths_self.ptrw() + old_size * sizeof(float_t), 0, bytes_to_clear * sizeof(float_t));
        memset(alignment_strengths_others.ptrw() + old_size * sizeof(float_t), 0, bytes_to_clear * sizeof(float_t));
        memset(separation_strengths_others.ptrw() + old_size * sizeof(float_t), 0, bytes_to_clear * sizeof(float_t));
        memset(cohesion_strengths_others.ptrw() + old_size * sizeof(float_t), 0, bytes_to_clear * sizeof(float_t));
        memset(speeds.ptrw() + old_size * sizeof(float_t), 0, bytes_to_clear * sizeof(float_t));
        memset(max_speeds.ptrw() + old_size * sizeof(float_t), 0, bytes_to_clear * sizeof(float_t));
    }
}

void BoidsDataBuffer::queue_boid_data(Boid* boid, uint32_t index) {
    if (!boid) return;

    // Queue all boid data as tasks
    std::lock_guard<std::mutex> lock(tasks_mutex);

    queued_tasks[{UNIT_ID, index}] = std::make_shared<BufferTask>(UNIT_ID, index, boid->get_unit_id());
    queued_tasks[{ATTRIBUTE, index}] = std::make_shared<BufferTask>(ATTRIBUTE, index, boid->get_attribute());
    queued_tasks[{POSITION, index}] = std::make_shared<BufferTask>(POSITION, index, boid->get_position());
    queued_tasks[{VELOCITY, index}] = std::make_shared<BufferTask>(VELOCITY, index, boid->get_velocity());
    queued_tasks[{ATTACK_VECTOR, index}] = std::make_shared<BufferTask>(ATTACK_VECTOR, index, boid->get_attack_vector());
    queued_tasks[{VIEW_RANGE, index}] = std::make_shared<BufferTask>(VIEW_RANGE, index, boid->get_view_range());
    queued_tasks[{COMBAT_RANGE, index}] = std::make_shared<BufferTask>(COMBAT_RANGE, index, boid->get_combat_range());
    queued_tasks[{COLLISION_RANGE, index}] = std::make_shared<BufferTask>(COLLISION_RANGE, index, boid->get_collision_range());
    queued_tasks[{VIEW_ANGLE, index}] = std::make_shared<BufferTask>(VIEW_ANGLE, index, boid->get_view_angle());
    queued_tasks[{ALIGNMENT_MASK, index}] = std::make_shared<BufferTask>(ALIGNMENT_MASK, index, boid->get_alignment_mask());
    queued_tasks[{SEPARATION_MASK, index}] = std::make_shared<BufferTask>(SEPARATION_MASK, index, boid->get_separation_mask());
    queued_tasks[{COHESION_MASK, index}] = std::make_shared<BufferTask>(COHESION_MASK, index, boid->get_cohesion_mask());
    queued_tasks[{ALIGNMENT_SELF, index}] = std::make_shared<BufferTask>(ALIGNMENT_SELF, index, boid->get_alignment_strength_self());
    queued_tasks[{SEPARATION_SELF, index}] = std::make_shared<BufferTask>(SEPARATION_SELF, index, boid->get_separation_strength_self());
    queued_tasks[{COHESION_SELF, index}] = std::make_shared<BufferTask>(COHESION_SELF, index, boid->get_cohesion_strength_self());
    queued_tasks[{ALIGNMENT_OTHER, index}] = std::make_shared<BufferTask>(ALIGNMENT_OTHER, index, boid->get_alignment_strength_others());
    queued_tasks[{SEPARATION_OTHER, index}] = std::make_shared<BufferTask>(SEPARATION_OTHER, index, boid->get_separation_strength_others());
    queued_tasks[{COHESION_OTHER, index}] = std::make_shared<BufferTask>(COHESION_OTHER, index, boid->get_cohesion_strength_others());
    queued_tasks[{SPEED, index}] = std::make_shared<BufferTask>(SPEED, index, boid->get_speed());
    queued_tasks[{MAX_SPEED, index}] = std::make_shared<BufferTask>(MAX_SPEED, index, boid->get_max_speed());
}

bool BoidsDataBuffer::is_active(uint32_t index) const {
    return index < buffer_size &&
        active_flags.size() > index * sizeof(uint32_t) &&
        reinterpret_cast<const uint32_t*>(active_flags.ptr())[index] != 0;
}

void BoidsDataBuffer::set_active_flag(uint32_t index, bool active) {
    if (index >= buffer_size) return;

    uint32_t* flags = reinterpret_cast<uint32_t*>(active_flags.ptrw());
    flags[index] = active ? 1 : 0;
}

void BoidsDataBuffer::apply_task_to_buffer(BufferTaskPtr task) {
    PackedByteArray& buffer = get_array_for_task_type(task->buffer_task_type);
    size_t data_size = get_data_size(task->buffer_task_type);

    uint8_t* buffer_ptr = buffer.ptrw() + task->index * data_size;

    switch (task->buffer_task_type) {
    case UNIT_ID:
    case ATTRIBUTE:
    case ALIGNMENT_MASK:
    case SEPARATION_MASK:
    case COHESION_MASK:
    case ACTIVE_FLAG:
    case SLEEPY_FLAG:
        memcpy(buffer_ptr, &task->value.uint_val, sizeof(uint32_t));
        break;

    case POSITION:
    case VELOCITY:
    case ATTACK_VECTOR:
        memcpy(buffer_ptr, &task->value.vector2_val, sizeof(Vector2));
        break;

    default:
        memcpy(buffer_ptr, &task->value.float_val, sizeof(float_t));
        break;
    }
}

void BoidsDataBuffer::write_to_buffer_direct(BufferTaskType type, uint32_t index, const Vector2& value) {
    if (index >= buffer_size) return;

    PackedByteArray& buffer = get_array_for_task_type(type);
    size_t data_size = get_data_size(type);

    uint8_t* buffer_ptr = buffer.ptrw() + index * data_size;
    memcpy(buffer_ptr, &value, sizeof(Vector2));
}

void BoidsDataBuffer::write_to_buffer_direct(BufferTaskType type, uint32_t index, float_t value) {
    if (index >= buffer_size) return;

    PackedByteArray& buffer = get_array_for_task_type(type);
    size_t data_size = get_data_size(type);

    uint8_t* buffer_ptr = buffer.ptrw() + index * data_size;
    memcpy(buffer_ptr, &value, sizeof(float_t));
}

void BoidsDataBuffer::write_to_buffer_direct(BufferTaskType type, uint32_t index, uint32_t value) {
    if (index >= buffer_size) return;

    PackedByteArray& buffer = get_array_for_task_type(type);
    size_t data_size = get_data_size(type);

    uint8_t* buffer_ptr = buffer.ptrw() + index * data_size;
    memcpy(buffer_ptr, &value, sizeof(uint32_t));
}