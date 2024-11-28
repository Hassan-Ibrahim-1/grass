#version 410 core

out vec4 FragColor;

in vec2 tex_coord;

uniform sampler2D cube_texture;
uniform vec4 color;

void main() {
    FragColor = color * texture(cube_texture, tex_coord);
}

