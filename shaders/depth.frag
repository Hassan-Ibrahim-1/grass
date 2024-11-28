#version 410 core

out vec4 FragColor;

uniform float near;
uniform float far;

float linearize_depth(float depth);

void main() {
    float depth = linearize_depth(gl_FragCoord.z) / far;
    FragColor = vec4(vec3(depth), 1.0f);
}

float linearize_depth(float depth) {
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}
 
