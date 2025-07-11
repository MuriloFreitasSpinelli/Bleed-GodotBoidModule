#[compute]
#version 450
layout(local_size_x = 64) in;

struct BoidsData {
    uint unit_id;
    uint attr;
    bool actv;
    vec2 pos;
    vec2 vel;
    vec2 atk_vec;
    bool sleepy;
    float view_range;
    float combat_range;
    float collision_range;
    float view_angle;
    uint alignment_mask;
    uint separation_mask;
    uint cohesion_mask;
    float alignment_strength_self;
    float separation_strength_self;
    float cohesion_strength_self;
    float alignment_strength_others;
    float separation_strength_others;
    float cohesion_strength_others;
    float speed;
    float max_speed;
};

layout(set = 0, binding = 0, std430) readonly restrict buffer BoidsDataBuffer {
    BoidsData boid_data[];
};

layout(set = 0, binding = 1, std430) writeonly restrict buffer Output {
    BoidsData output_data[];
};

void main() {
    uint id = gl_GlobalInvocationID.x;
    output_data[id] = boid_data[id];
}