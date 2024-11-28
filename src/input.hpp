#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <vector>
#include "keycodes.hpp"

#define START_KEY_INDEX GLFW_KEY_SPACE // 32
#define MAX_KEYS (GLFW_KEY_LAST + 1) // 349

enum class MouseButton {
    ANY,
    RIGHT,
    LEFT
};

namespace input {

inline ImGuiIO* io = nullptr;

// used for determing mouse_button_just_clicked
// set to glfwGetTime after any click
// NOTE: mouse input uses a sticky key type implementation
inline float _lmb_click_start_time = 0.0f;
inline float _rmb_click_start_time = 0.0f;
inline float _lmb_release_start_time = 0.0f;
inline float _rmb_release_start_time = 0.0f;
// How long it should wait before setting lmb or rmb clicked to false
// How long it should wait to discard mouse input if it hasn't been read
static constexpr float _mouse_discard_wait_time = 0.1f;


// couldn't these just be _mouse_pos?
inline float _last_mouse_x = 0.0f;
inline float _last_mouse_y = 0.0f;
inline bool _first_mouse = true; // corrects for a large initial jump

inline glm::vec2 _mouse_pos = glm::vec2(0);
// NOTE: clicked and released bools are only true one frame
inline bool _lmb_down = false;
inline bool _lmb_clicked = false;
inline bool _lmb_released = false;
inline bool _rmb_down = false;
inline bool _rmb_clicked = false;
inline bool _rmb_released = false;

inline std::array<bool, MAX_KEYS> _key_pressed;
inline std::array<bool, MAX_KEYS> _key_released;
inline std::array<bool, MAX_KEYS> _key_down;
inline std::vector<GLFWkeyfun> _user_key_callbacks;

// ** PUBLIC **

// NOTE: call this every frame if you need mouse input
void update(GLFWwindow* window);

glm::vec2 get_mouse_pos();
float get_mouse_pos_x();
float get_mouse_pos_y();

// NOTE: All these functions disable their bools if they are true on read
// If checking if something is being clicked on check if the mouse is hovering on that thing
// before calling one of these functions. idk why it's this way. if done otherwise 
// any further checks won't work in the same frame? I think
bool mouse_button_down(MouseButton button = MouseButton::ANY);
bool mouse_button_clicked(MouseButton button = MouseButton::ANY);
bool mouse_button_released(MouseButton button = MouseButton::ANY);

void add_key_callback(GLFWkeyfun callback);

bool key_pressed(Key key);
bool key_released(Key key);
bool key_down(Key key);

// ** PRIVATE **

void update_mouse_buttons();
void update_keys();

// callbacks
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_movement_callback(GLFWwindow* window, double posx, double posy);
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

}

