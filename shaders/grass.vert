#version 410 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
// layout (location = 2) in mat4 a_tex_coords;
layout (location = 3) in mat4 a_model;
layout (location = 7) in mat4 a_inverse_model;

out vec3 normal;
out vec3 frag_pos; // fragment position

// layout (std140) uniform matrices {
//     mat4 projection;
//     mat4 view;
// };

// at most N grass
// #define MAX_GRASS 100
// uniform mat4 models[MAX_GRASS];
// uniform mat3 inverse_models[MAX_GRASS];
uniform mat4 projection;
uniform mat4 view;
uniform float time;

// eh
float random2d(vec2 coord) {
    return fract(sin(dot(coord.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    const float mult = 0.22;
    float offset = 0;
    // Only offset position if it is the middle 2 vertices
    if (a_position.y >= 0.5f) {
        float random = random2d(a_position.xy);
        offset = mult * sin(time) - (a_position.x * 0.1 + random * 0.1);
        if (a_position.y == 1.0f) {
            offset = ((offset + 0.5) + (offset - 0.5));
        }
    }

    vec3 position = (a_model * vec4(a_position, 1.0f)).xyz;
    position.x += offset;

    gl_Position = projection * view * vec4(position, 1);
    // gl_Position = vec4(0, 0.1, 0, 1);

    frag_pos = vec3(a_model * vec4(a_position, 1.0f));
    // mat3 inverse_model = mat3(transpose(inverse(a_model)));
    mat3 inverse_model = mat3(a_inverse_model);
    normal = normalize(inverse_model * a_normal);
}


