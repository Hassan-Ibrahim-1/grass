#version 410 core

// Mirrors the environment map

out vec4 FragColor;

in vec3 pos;
in vec3 normal;

uniform samplerCube skybox;
uniform vec3 camera_pos;

void main() {
    // incident vector
    vec3 I = normalize(pos - camera_pos);
    vec3 R = reflect(I, normalize(normal));
    FragColor = vec4(texture(skybox, R).rgb, 1);
}

