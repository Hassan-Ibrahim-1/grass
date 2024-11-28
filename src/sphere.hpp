#pragma once

#include "game_object.hpp"

struct Sphere : GameObject {
    // NOTE: draw command set in scene
    Sphere() {}
    Sphere(Transform transform, Material material)
        : GameObject(transform, material) {
    }
    Sphere(glm::vec3 position, glm::vec3 scale = glm::vec3(1), Material material = Material())
        : GameObject(Transform(position, scale), material) {}
};

