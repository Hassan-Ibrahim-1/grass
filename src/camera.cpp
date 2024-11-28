#include <glm/gtc/matrix_transform.hpp>

#include "debug.hpp"
#include "camera.hpp"
#include "imgui.h"
#include "transform.hpp"

Camera::Camera(Transform transform, glm::vec3 world_up)
    : transform(transform),
      world_up(world_up),
      up(world_up) {
}

Camera::Camera(glm::vec3 position, Rotation rotation, glm::vec3 scale, glm::vec3 world_up)
    : Camera(Transform(position, scale, rotation), world_up) {
}

glm::mat4 Camera::get_view_matrix() {
    return glm::lookAt(transform.position, transform.position + front, up);
}

glm::mat4 Camera::get_perspective_matrix() {
    return
        glm::perspective(
            glm::radians(fov),
            aspect_ratio,
            near,
            far
        );
}

void Camera::process_keyboard(CameraDirection direction, float delta_time) {
    if (_locked) {
        return;
    }
    static const float vertical_multiplier = 5.0f;

    float speed = velocity * delta_time;

    switch (direction) {
    case CameraDirection::FORWARD:
        transform.position += front * speed;
        break;
    case CameraDirection::BACKWARD:
        transform.position -= front * speed;
        break;
    case CameraDirection::RIGHT:
        transform.position += right * speed;
        break;
    case CameraDirection::LEFT:
        transform.position -= right * speed;
        break;
    case CameraDirection::UP:
        transform.position += world_up * (speed * vertical_multiplier);
        break;
    case CameraDirection::DOWN:
        transform.position -= world_up * (speed * vertical_multiplier);
        break;
    }
}

void Camera::process_mouse_movement(float x_offset, float y_offset, bool constrain_pitch, bool invert_pitch) {
    if (_locked) {
        return;
    }
    transform.rotation.yaw += (x_offset * sensitivity.x);
    transform.rotation.pitch += ((invert_pitch ? y_offset : -y_offset) * sensitivity.y);
    /*transform.rotation.yaw += x_offset * sensitivity.x;*/

    if (constrain_pitch) {
        if (transform.rotation.pitch > 89.0f) {
            transform.rotation.pitch = 89.0f;
        }
        else if (transform.rotation.pitch < -89.0f) {
            transform.rotation.pitch = -89.0f;
        }
    }
    update_vectors();
}

void Camera::process_mouse_scroll(float y_offset) {
    fov -= y_offset;
    if (fov > 100.0f) {
        fov = 100.0f;
    }
    else if (fov < 45.0f) {
        fov = 45.0f;
    }
}

void Camera::look_back() {
    if (_reset_look_back) {
        transform.rotation.yaw -= 180;
    }
    else {
        transform.rotation.yaw += 180;
    }
    _reset_look_back = !_reset_look_back;
    update_vectors();
}

void Camera::set_rotation(float yaw, float pitch) {
    transform.rotation.yaw = yaw;
    transform.rotation.pitch = pitch;
    update_vectors();
}

void Camera::update_vectors() {
    // Calculate new front vector - where the camera is facing
    glm::vec3 tmp_front;
    tmp_front.x = cos(glm::radians(transform.rotation.yaw)) * cos(glm::radians(transform.rotation.pitch));
    tmp_front.y = sin(glm::radians(transform.rotation.pitch));
    tmp_front.z = sin(glm::radians(transform.rotation.yaw)) * cos(glm::radians(transform.rotation.pitch));
    front = glm::normalize(tmp_front);
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}
void Camera::lock() {
    _locked = true;
}
void Camera::unlock() {
    _locked = false;
}

bool Camera::locked() const {
    return _locked;
}

