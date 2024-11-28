#pragma once

#include <glm/glm.hpp>

struct Rotation {
    float yaw;
    float pitch;
    float roll;

    Rotation(float yaw = 0.0f, float pitch = 0.0f, float roll = 0.0f);

    friend bool operator==(const Rotation& r1, const Rotation& r2);
    friend bool operator!=(const Rotation& r1, const Rotation& r2);
};

struct Transform {
public:
    glm::vec3 position;
    glm::vec3 scale = glm::vec3(1);
    Rotation rotation;

    Transform(glm::vec3 position = glm::vec3(),
              glm::vec3 scale = glm::vec3(1),
              Rotation rotation = Rotation());

    // only factors in position and scale
    glm::mat4 get_mat4() const;

    friend bool operator==(const Transform& t1, const Transform& t2);
    friend bool operator!=(const Transform& t1, const Transform& t2);
};

