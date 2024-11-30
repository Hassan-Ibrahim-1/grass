#version 410 core

// Mirrors the environment map

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
// layout (location = 2) in vec2 a_tex_coord;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

uniform mat4 model;
uniform mat3 inverse_model;

out vec3 pos;
out vec3 normal;

void main() {
    gl_Position = projection * view * model * vec4(a_position, 1.0f);
    normal = normalize(inverse_model * a_normal);
    pos = vec3(model * vec4(a_position, 1));
}


