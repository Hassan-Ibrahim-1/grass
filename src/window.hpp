#pragma once

#include <string>

#include <GLFW/glfw3.h>

#include "common.hpp"

class Window {
public:
    Window();
    Window(uint width, uint height, const std::string& title);
    ~Window();
    
    uint width() const;
    uint height() const;
    GLFWwindow* data();

    bool in_focus() const;
    bool should_close() const;
    void enable_cursor();
    void disable_cursor();

    
private:
    GLFWwindow* _window = nullptr;
    bool _loaded = false;

    uint _width = 0;
    uint _height = 0;
    std::string _title;

    GLFWwindow* create_window();
    void load_opengl_functions();
};

