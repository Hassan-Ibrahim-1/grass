#version 410 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
// layout (location = 2) in vec2 a_tex_coord;

out vec3 normal;
out vec3 frag_pos; // fragment position

layout (std140) uniform matrices {
    mat4 projection;
    mat4 view;
};

// at most N grass
#define MAX_GRASS 10 
uniform mat4 models[MAX_GRASS];
uniform mat3 inverse_models[MAX_GRASS];
uniform float time;

// eh
float random2d(vec2 coord) {
    return fract(sin(dot(coord.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    const float mult = 0.022;
    float offset = 0;
    // Only offset position if it is the middle 2 vertices
    if (a_position.y >= 0.5f) {
        offset = mult * cos(time);
        if (a_position.y == 1.0f) {
            offset = ((offset + 0.5) + (offset - 0.5));
        }
    }

    vec3 position = (models[gl_InstanceID] * vec4(a_position, 1.0f)).xyz;
    position.x += offset;

    gl_Position = projection * view * vec4(position, 1);

    frag_pos = vec3(models[gl_InstanceID] * vec4(a_position, 1.0f));
    normal = normalize(inverse_models[gl_InstanceID] * a_normal);
}

