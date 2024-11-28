#pragma once

#include <glm/glm.hpp>

#include "imgui.h"
#include "transform.hpp"

enum class CameraDirection {
    FORWARD = 0,
    BACKWARD,
    RIGHT,
    LEFT,
    UP,
    DOWN,
};

// FIXME: Why does this exist?
namespace CameraDefaults {
    constexpr float fov = 45.0f;
    const glm::vec2 sensitivity(0.1f, 0.1f);
    constexpr float velocity = 7.0f;
    const Rotation rotation(-90.0f, 0.0f, 0.0f);
    const glm::vec3 front(0.0f, 0.0f, -1.0f);
    constexpr float near = 0.1f;
    constexpr float far = 100.0f;
    constexpr float aspect_ratio = (float)16 / 9;
}

class Camera {
public:
    Transform transform = {
        glm::vec3(0),
        glm::vec3(1),
        CameraDefaults::rotation
    };

    glm::vec3 world_up;

    glm::vec3 up;
    glm::vec3 front = CameraDefaults::front;
    glm::vec3 right;

    float fov = CameraDefaults::fov;
    glm::vec2 sensitivity = CameraDefaults::sensitivity;
    float velocity = CameraDefaults::velocity;

    float near = CameraDefaults::near;
    float far  = CameraDefaults::far;
    float aspect_ratio = CameraDefaults::aspect_ratio;

    // Transform init
    Camera(Transform transform,
           glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f));

    // NOTE: default constructor
    // vector init
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           Rotation rotation = CameraDefaults::rotation,
           glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
           glm::vec3 world_up = glm::vec3(0.0f, 0.1f, 0.0f));

    glm::mat4 get_view_matrix();
    glm::mat4 get_perspective_matrix();

    void process_keyboard(CameraDirection direction, float delta_time);
    void process_mouse_movement(float x_offset, float y_offset, bool constrain_pitch = true, bool invert_pitch = false);
    void process_mouse_scroll(float y_offset);

    // Points the camera backwords
    void look_back();
    void set_rotation(float yaw, float pitch);

    void update_vectors();

    // Locks the camera at the current position and view direviton
    void lock();
    void unlock();

    bool locked() const;

private:
    bool _reset_look_back = false;
    bool _locked = false;
};

