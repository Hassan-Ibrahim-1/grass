#version 410 core

// Mirrors the environment map

out vec4 FragColor;

in vec3 pos;
in vec3 normal;

uniform samplerCube skybox;
uniform vec3 camera_pos;

void main() {
    // ni * sin 0i = nr * sin 0r
    // ni / nr
    // act as a magnifying glass.
    // higher magnification when the ratio is smaller
    const float ratio = 1 / 1.1;
    // incident vector
    vec3 I = normalize(pos - camera_pos);
    vec3 R = refract(I, normalize(normal), ratio);
    // R = reflect(I, normalize(normal));
    FragColor = vec4(texture(skybox, R).rgb, 1);
}

