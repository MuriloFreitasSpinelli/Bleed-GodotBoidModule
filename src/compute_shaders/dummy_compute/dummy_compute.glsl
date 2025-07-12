#[compute]
#version 450
layout(local_size_x = 64) in;

layout(set = 0, binding = 0, std430) readonly restrict buffer BoidIdentity {
    uint unit_id[];
    uint attr[];
} boid_identity;

layout(set = 0, binding = 1, std430) readonly restrict buffer BoidMode {
    uint activ[];
    uint sleepy[];
} boid_mode;

layout(set = 0, binding = 2, std430) readonly restrict buffer BoidVector {
    vec2 pos[];
    vec2 vel[];
    vec2 atk_vec[];
} boid_vector;

layout(set = 0, binding = 3, std430) readonly restrict buffer BoidRange {
    float view_range[];
    float combat_range[];
    float collision_range[];
    float view_angle[];
} boid_range;

layout(set = 0, binding = 4, std430) readonly restrict buffer BoidMask {
    uint alignment_mask[];
    uint separation_mask[];
    uint cohesion_mask[];
} boid_mask;

layout(set = 0, binding = 5, std430) readonly restrict buffer BoidSelfStrength {
    float alignment_strength_self[];
    float separation_strength_self[];
    float cohesion_strength_self[];
} boid_self_strength;

layout(set = 0, binding = 6, std430) readonly restrict buffer BoidOtherStrength {
    float alignment_strength_others[];
    float separation_strength_others[];
    float cohesion_strength_others[];
} boid_other_strength;

layout(set = 0, binding = 7, std430) readonly restrict buffer BoidSpeed {
    float speed[];
    float max_speed[];
} boid_speed;

layout(set = 0, binding = 8, std430) writeonly restrict buffer Output {
    vec2 output_data[];
} outp;

void main() {
    uint id = gl_GlobalInvocationID.x;
    outp.output_data[id] = boid_vector.vel[id] * 2;
}