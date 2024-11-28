// a textured rect shader that expects positions in screen space
#version 410 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_tex_coord;

out vec2 tex_coord;

void main() {
    gl_Position = vec4(a_position.xy, 0.0, 1.0f);
    tex_coord = a_tex_coord;
}

