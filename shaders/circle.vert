#version 410 core

layout (location = 0) in vec3 a_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // gl_Position = vec4(0, 0, 0, 0);
    gl_Position = projection * view * model * vec4(a_position, 1.0f);
}

