#pragma once

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position = glm::vec3(0);
    glm::vec3 normal = glm::vec3(0);
    glm::vec2 tex_coords = glm::vec2(0);

    Vertex() = default;
    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 tex_coords);
};

