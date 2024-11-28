#version 410 core

out vec4 FragColor;

in vec2 tex_coord;

uniform sampler2D rect_texture;
uniform vec3 color;

void main() {
    // FragColor = vec4(color, 1);
    FragColor = vec4(color, 1) * texture(rect_texture, tex_coord);
    /*
    if (FragColor.a < 0.1) {
        discard;
    }
    */
    // FragColor = texture(rect_texture, tex_coord);    
}

