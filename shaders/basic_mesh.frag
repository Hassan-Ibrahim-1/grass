#version 410 core

out vec4 FragColor;

struct Material {
    vec3 color;
};

uniform Material material;

void main() {
    FragColor = vec4(material.color, 1.0f);
    // FragColor = vec4(1, 0, 0, 1);
}

