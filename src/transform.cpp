#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "transform.hpp"

Rotation::Rotation(float yaw, float pitch, float roll)
    : yaw(yaw),
      pitch(pitch),
      roll(roll) {}

Transform::Transform(glm::vec3 position, glm::vec3 scale, Rotation rotation)
    : position(position),
      rotation(rotation),
      scale(scale) {}

glm::mat4 Transform::get_mat4() const {
    glm::mat4 mat(1);
    mat = glm::translate(mat, position);
    mat = glm::scale(mat, scale);

    mat = glm::rotate(mat, glm::radians(rotation.yaw), glm::vec3(0, 1, 0));
    mat = glm::rotate(mat, glm::radians(rotation.pitch), glm::vec3(1, 0, 0));
    mat = glm::rotate(mat, glm::radians(rotation.roll), glm::vec3(0, 0, 1));

    return mat;
}

bool operator==(const Rotation& r1, const Rotation& r2) {
    return (r1.yaw == r2.yaw)
        && (r1.pitch == r2.pitch)
        && (r1.roll == r2.roll);
}

bool operator!=(const Rotation& r1, const Rotation& r2) {
    return (r1.yaw != r2.yaw)
        || (r1.pitch != r2.pitch)
        || (r1.roll != r2.roll);
}

bool operator==(const Transform& t1, const Transform& t2) {
    return (t1.position == t2.position)
        && (t1.scale == t2.scale)
        && (t1.rotation == t2.rotation);
}

bool operator!=(const Transform& t1, const Transform& t2) {
    return (t1.position != t2.position)
        || (t1.scale != t2.scale)
        || (t1.rotation != t2.rotation);
}

