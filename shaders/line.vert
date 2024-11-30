#version 410 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

out vec4 color;

void main() {
    gl_Position = projection * view * vec4(a_position, 1.0f);
    color = a_color;
}

