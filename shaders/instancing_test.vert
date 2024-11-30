#version 410 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec3 a_color;

out vec3 color;

uniform vec2 offsets[100];

void main() {
    gl_Position = vec4(a_position + offsets[gl_InstanceID], 0, 1);
    color = a_color;
}

