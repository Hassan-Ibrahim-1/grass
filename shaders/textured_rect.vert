#version 410 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_tex_coord;

out vec2 tex_coord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(a_position, 1.0f);
    tex_coord = a_tex_coord;
}

