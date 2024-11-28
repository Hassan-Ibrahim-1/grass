#version 410 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coords;

out vec3 normal;
out vec3 frag_pos; // fragment position
out vec2 tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(a_position, 1.0f);
    frag_pos = vec3(view * model * vec4(a_position, 1.0f));
    // normal = a_normal;
    normal = mat3(transpose(inverse(view * model))) * a_normal;
    tex_coords = a_tex_coords;
}

