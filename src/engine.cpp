#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "engine.hpp"
#include "fs.hpp"
#include "input.hpp"
#include "debug.hpp"
#include "renderer.hpp"
#include "utils.hpp"

void engine::run(Application& app) {
    _app = &app;

    _app->init();
    while (!_window->should_close()) {
        // Don't update outside of focus if true
        if (!update_outside_focus) {
            if (!_window->in_focus()) {
                ImGui_ImplGlfw_Sleep(10);
                continue;
            }
        }
        update();
    }
    cleanup();
}

float& engine::get_delta_time() {
    return _delta_time;
}

Window& engine::get_window() {
    return *_window;
}

Renderer& engine::get_renderer() {
    return *_renderer;
}

Camera& engine::get_camera() {
    return _camera;
}

Scene& engine::get_scene() {
    return _scene;
}

void engine::init_glfw() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    #endif
}

void engine::init_window(uint width, uint height, const std::string& title) {
    _window = std::make_unique<Window>(width, height, title);
    if (cursor_enabled) {
        _window->enable_cursor();
    }
    else {
        _window->disable_cursor();
    }
}

void engine::init_imgui() {
    // imgui stuff
    const char* glsl_version = "#version 410";
    
    // imgui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io; // what
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    input::io = &io;

    ImGui::StyleColorsDark();

    // setup renderer
    ImGui_ImplGlfw_InitForOpenGL(_window->data(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void engine::init_input_handler_callbacks() {
    glfwSetKeyCallback(_window->data(), input::key_callback);
    glfwSetCursorPosCallback(_window->data(), input::mouse_movement_callback);
    glfwSetScrollCallback(_window->data(), input::mouse_scroll_callback);
    glfwSetMouseButtonCallback(_window->data(), input::mouse_button_callback);

    input::add_key_callback(default_key_callback);
}

void engine::default_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        _renderer->wireframe_enabled = !_renderer->wireframe_enabled;
    }
    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        _renderer->reload_shaders();
    }
    // enable / disable cursor
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        cursor_enabled = !cursor_enabled;
    }
    if ((key == GLFW_KEY_M || key == GLFW_KEY_GRAVE_ACCENT)&& action == GLFW_PRESS) {
        _show_default_imgui_window = !_show_default_imgui_window;
    }
}

void engine::init(const EngineInitInfo& cinfo) {
    // HACK: fixes CameraDefaults::rotation not being set for some
    // reason. this causes the front vector to be 1, 0, 0 instead of
    // what its supposed to be which is 0, 0, -1.
    // Figure out how to not have to do this
    _camera.set_rotation(-90, 0);

    utils::init();
    fs::init("shaders", "models");
    init_glfw();

    ASSERT(
       cinfo.window_width != -1 && cinfo.window_height != -1,
       "Window width and height not provided in EngineCreateInfo"
    );
    init_window(cinfo.window_width, cinfo.window_height, cinfo.window_title);

    // init member variables
    imgui_enabled = cinfo.imgui_enabled;
    hide_imgui_windows = cinfo.hide_imgui_windows;
    cursor_enabled = cinfo.cursor_enabled;
    update_outside_focus = cinfo.update_outside_focus;
    render_after_user_update = cinfo.render_after_user_update;
    clear_color = cinfo.clear_color;

    if (cinfo.set_input_handler_callbacks) {
        init_input_handler_callbacks();
    }
    if (cinfo.vsync_enabled) {
        glfwSwapInterval(1);
    }
    else {
        glfwSwapInterval(0);
    }
    if (imgui_enabled) {
        init_imgui();
    }

    _renderer = std::make_unique<Renderer>(_camera, _scene);
}

void engine::poll_input() {
    glfwPollEvents();
    input::update(_window->data());
}

void engine::imgui_new_frame() {
    // start imgui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void engine::imgui_render() {
    // render imgui windows
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void engine::show_default_imgui_window() {
    if (_show_default_imgui_window && cursor_enabled) {
        ImGui::Begin("Settings", &_show_default_imgui_window);
        if (ImGui::TreeNode("Renderer")) {
            ImGui::Checkbox("depth view", &_renderer->depth_view_enabled);
            ImGui::Spacing();
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Camera")) {
            if (utils::imgui_transform("camera", _camera.transform)) {
                _camera.update_vectors();
            }
            ImGui::DragFloat("speed", &_camera.velocity);
            ImGui::DragFloat("frustum near", &_camera.near);
            ImGui::DragFloat("frustum far", &_camera.far);
            ImGui::Spacing();
            ImGui::TreePop();
        }
        utils::imgui_color_edit4("clear color", clear_color);
        utils::imgui_fps_text();
        ImGui::End();
    }
}

void engine::clear_screen() {
    auto clamped_color = clear_color.clamped_vec3();
    glClearColor(clamped_color.r, clamped_color.g, clamped_color.b, 1.0f);
    // TODO: Would it be better to only clear enabled buffers?
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void engine::update_delta_time() {
    float current_frame = glfwGetTime();
    _delta_time = current_frame - _last_frame_time;
    _last_frame_time = current_frame;
}

void engine::update() {
    update_delta_time();
    poll_input();
    if (imgui_enabled) {
        imgui_new_frame();
    }
    clear_screen();


    if (cursor_enabled) {
        _window->enable_cursor();
    }
    else {
        _window->disable_cursor();
    }

    // user update
    _app->update();

    if (render_after_user_update) {
        _renderer->render();
    }

    if (imgui_enabled && !hide_imgui_windows) {
        show_default_imgui_window();
        imgui_render();
    }

    glfwSwapBuffers(_window->data());
}

void engine::cleanup() {
    _app->cleanup();
    // _scene.clear_game_objects();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    LOG("TERMINATED");
}

