#version 410 core

out vec4 FragColor;

// in vec3 normal;
// in vec2 tex_coord;

uniform vec4 color;

void main() {
    // TODO: lighting
    // FragColor = vec4(0.04, 0.28, 0.26, 1.0);
    FragColor = color;
}

