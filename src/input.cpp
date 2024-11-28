#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "input.hpp"
#include "camera.hpp"
#include "engine.hpp"
#include "debug.hpp"

void input::update(GLFWwindow* window) {
    update_mouse_buttons();
    update_keys();

    Camera& camera = engine::get_camera();
    float delta_time = engine::get_delta_time();

    if (io->WantTextInput) return;

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.process_keyboard(CameraDirection::UP, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.process_keyboard(CameraDirection::DOWN, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.process_keyboard(CameraDirection::FORWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.process_keyboard(CameraDirection::BACKWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.process_keyboard(CameraDirection::RIGHT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.process_keyboard(CameraDirection::LEFT, delta_time);
    }
}

glm::vec2 input::get_mouse_pos() {
    return _mouse_pos;
}

float input::get_mouse_pos_x() {
    return _mouse_pos.x;
}
float input::get_mouse_pos_y() {
    return _mouse_pos.y;
}

void input::update_mouse_buttons() {
    if (_lmb_clicked) {
        if (_lmb_click_start_time + _mouse_discard_wait_time < glfwGetTime()) {
            _lmb_clicked = false;
        }
    }
    if (_rmb_clicked) {
        if (_rmb_click_start_time + _mouse_discard_wait_time < glfwGetTime()) {
            _rmb_clicked = false;
        }
    }
    if (_lmb_released) {
        if (_lmb_release_start_time + _mouse_discard_wait_time < glfwGetTime()) {
            _lmb_released = false;
        }
    }
    if (_rmb_released) {
        if (_rmb_release_start_time + _mouse_discard_wait_time < glfwGetTime()) {
            _rmb_released = false;
        }
    }
}

void input::update_keys() {
    for (uint i = 32; i < MAX_KEYS; i++) {
        _key_pressed[i] = false;
        _key_released[i] = false;

        bool down = glfwGetKey(engine::get_window().data(), i) == GLFW_PRESS;
        // If not down last frame and down now - pressed
        if (!_key_down[i] && down) {
            _key_pressed[i] = true;
        }
        // Down last frame and not down now - released
        else if (_key_down[i] && !down) {
            _key_released[i] = true;
        }
        _key_down[i] = down;
    }
}

bool input::key_pressed(Key key) {
    return _key_pressed[static_cast<int>(key)];
}

bool input::key_released(Key key) {
    return _key_released[static_cast<int>(key)];
}

bool input::key_down(Key key) {
    return _key_down[static_cast<int>(key)];
}

void input::add_key_callback(GLFWkeyfun callback) {
    _user_key_callbacks.emplace_back(callback);
}

void input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if (engine::cursor_enabled) {
            engine::cursor_enabled = false;
        }
        else {
            glfwSetWindowShouldClose(window, true);
        }
    }

    if (io->WantTextInput) {
        return;
    }
    
    for (auto callback : _user_key_callbacks) {
        callback(window, key, scancode, action, mods);
    }
}

void input::mouse_movement_callback(GLFWwindow* window, double posx, double posy) {

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    /*global::mouse_pos.x = posx / width;*/
    /*global::mouse_pos.y = posy / height;*/

    glm::vec3 win(posx, posy, 0);
    glm::vec4 viewport(0, 0, width, height);
    glm::vec3 real_pos =
        glm::unProject(win, glm::mat4(1.0f), glm::mat4(1.0f), viewport);

    _mouse_pos.x = real_pos.x;
    _mouse_pos.y = -real_pos.y;

    if (engine::cursor_enabled) {
        _last_mouse_x = posx;
        _last_mouse_y = posy;
        return;
    }
    if (_first_mouse) {
        _last_mouse_x = posx;
        _last_mouse_y = posy;
        _first_mouse = false;
    }
    float x_offset = posx - _last_mouse_x;
    float y_offset = posy - _last_mouse_y;
    _last_mouse_x = posx;
    _last_mouse_y = posy;

    engine::get_camera().process_mouse_movement(x_offset, y_offset);
}

void input::mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (engine::cursor_enabled) return;

    engine::get_camera().process_mouse_scroll(yoffset);
}

void input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (io->WantCaptureMouse) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            _lmb_down = true;
            _lmb_clicked = true;
            _lmb_released = false;
            _lmb_click_start_time = glfwGetTime();
        }
        else if (action == GLFW_RELEASE) {
            _lmb_down = false;
            _lmb_clicked = false;
            _lmb_released = true;
            _lmb_release_start_time = glfwGetTime();
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            _rmb_down = true;
            _rmb_clicked = true;
            _rmb_released = false;
            _rmb_click_start_time = glfwGetTime();
        }
        else if (action == GLFW_RELEASE) {
            _rmb_down = false;
            _rmb_clicked = false;
            _rmb_released = true;
            _rmb_release_start_time = glfwGetTime();
        }
    }
}

bool input::mouse_button_down(MouseButton button) {
    if (button == MouseButton::ANY) {
        return _lmb_down || _rmb_down;
    }
    else if (button == MouseButton::LEFT) {
        return _lmb_down;
    }
    else if (button == MouseButton::RIGHT) {
        return _rmb_down;
    }
    /*if (button == MouseButton::ANY) {*/
    /*    if (_lmb_down) {*/
    /*        _lmb_down = false;*/
    /*        return true;*/
    /*    }*/
    /*    if (_rmb_down) {*/
    /*        _rmb_down = false;*/
    /*        return true;*/
    /*    }*/
    /*    return false;*/
    /*}*/
    /*else if (button == MouseButton::LEFT) {*/
    /*    if (_lmb_down) {*/
    /*        _lmb_down = false;*/
    /*        return true;*/
    /*    }*/
    /*    return false;*/
    /*}*/
    /*else if (button == MouseButton::RIGHT) {*/
    /*    if (_rmb_down) {*/
    /*        _rmb_down = false;*/
    /*        return true;*/
    /*    }*/
    /*    return false;*/
    /*}*/
    /**/
    // never reached
    return false;
}

bool input::mouse_button_clicked(MouseButton button) {
    if (button == MouseButton::ANY) {
        if (_lmb_clicked) {
            _lmb_clicked = false;
            return true;
        }
        if (_rmb_clicked) {
            _rmb_clicked = false;
            return true;
        }
        return false;
    }
    else if (button == MouseButton::LEFT) {
        if (_lmb_clicked) {
            _lmb_clicked = false;
            return true;
        }
        return false;
    }
    else if (button == MouseButton::RIGHT) {
        if (_rmb_clicked) {
            _rmb_clicked = false;
            return true;
        }
        return false;
    }
    
    // never reached
    return false;
}

bool input::mouse_button_released(MouseButton button) {
    if (button == MouseButton::ANY) {
        if (_lmb_released) {
            _lmb_released = false;
            return true;
        }
        if (_rmb_released) {
            _rmb_released = false;
            return true;
        }
        return false;
    }
    else if (button == MouseButton::LEFT) {
        if (_lmb_released) {
            _lmb_released = false;
            return true;
        }
        return false;
    }
    else if (button == MouseButton::RIGHT) {
        if (_rmb_released) {
            _rmb_released = false;
            return true;
        }
        return false;
    }
    
    // never reached
    return false;
}

