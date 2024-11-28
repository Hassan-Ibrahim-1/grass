#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "debug.hpp"

#include "window.hpp"

static void default_framebuffer_size_callback(GLFWwindow* window, int width, int height);

Window::Window(uint width, uint height, const std::string& title)
    : _width(width), _height(height), _title(title) {
    _window = create_window();
    glfwMakeContextCurrent(_window);
    // Sets a default basic frame buffer size callback
    glfwSetFramebufferSizeCallback(_window, default_framebuffer_size_callback);
    load_opengl_functions();
    _loaded = true;
}

Window::~Window() {
    if (_loaded) {
        glfwDestroyWindow(_window);
    }
}

uint Window::width() const {
    return _width;
}

uint Window::height() const {
    return _height;
}

GLFWwindow* Window::data() {
    return _window;
}

bool Window::in_focus() const {
    return glfwGetWindowAttrib(_window, GLFW_ICONIFIED) == 0;
}

bool Window::should_close() const {
    return glfwWindowShouldClose(_window);
}

void Window::enable_cursor() {
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::disable_cursor() {
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

GLFWwindow* Window::create_window() {
    GLFWwindow* window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);
    ASSERT(window != nullptr, "Bad window intialization");
    return window;
}

void Window::load_opengl_functions() {
    ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Bad glad initialization");
}

static void default_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

