#version 410 core

out vec4 FragColor;

in vec3 normal;

uniform vec4 color;

void main() {
    // FragColor = color;
    FragColor = vec4(normal, 1.0f);
    // FragColor = vec4(1, 1, 1, 1);
}

