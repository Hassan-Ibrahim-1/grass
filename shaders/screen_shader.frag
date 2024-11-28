#version 410 core

out vec4 FragColor;

in vec2 tex_coord;

uniform sampler2D screen_texture;

const float offset = 1.0 / 300.0;

void main() {
    vec2 offsets[9] = vec2[] (
        vec2(-offset, offset), // top-left
        vec2(0.0f, offset), // top-center
        vec2(offset, offset), // top-right
        vec2(-offset, 0.0f), // center-left
        vec2(0.0f, 0.0f), // center-center
        vec2(offset, 0.0f), // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f, -offset), // bottom-center
        vec2(offset, -offset) // bottom-right
    );

    float sharpen_kernel[9] = float[] (
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    float blur_kernel[9] = float[] (
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16
    );

    float edge_detection_kernel[9] = float[] (
        1,  1, 1,
        1, -8, 1,
        1,  1, 1
    );

    float test_kernel[9] = float[] (
        0, -1, 0,
        -1, 4, -1,
        0, -1, 0
    );

    // Sample surrounding texture values
    vec3 sample_tex[9];
    for (int i = 0; i < 9; i++) {
        sample_tex[i] = vec3(texture(screen_texture, tex_coord.st + offsets[i]));
    }

    vec3 color = vec3(0.0f);
    // multiply sampled texture colors with its corresponding kernel value
    // then add everything up
    for (int i = 0; i < 9; i++) {
        // color += sample_tex[i] * sharpen_kernel[i];
        // color += sample_tex[i] * blur_kernel[i];
        // color += sample_tex[i] * test_kernel[i]; // edge detection
        // color += sample_tex[i] * edge_detection_kernel[i];
    }

    FragColor = vec4(color, 1.0f);
    FragColor = texture(screen_texture, tex_coord);
}

