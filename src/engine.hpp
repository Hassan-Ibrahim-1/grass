#pragma once

#include "input.hpp"
#include "renderer.hpp"
#include "scene.hpp"
#include "window.hpp"

class Application;

struct EngineInitInfo {
    uint window_width = -1;
    uint window_height = -1;
    std::string window_title = "Bebe";

    bool vsync_enabled = false;
    bool imgui_enabled = true;
    bool set_input_handler_callbacks = true;
    bool hide_imgui_windows = false;
    bool cursor_enabled = true;
    bool update_outside_focus = true;
    bool render_after_user_update = true;
    Color clear_color = Color(0, 0, 0);
};

namespace engine {

// ** PUBLIC **

inline bool imgui_enabled;
inline bool hide_imgui_windows;
inline bool cursor_enabled;
inline bool update_outside_focus;
inline bool render_after_user_update;
inline Color clear_color;

void init(const EngineInitInfo& cinfo);
void run(Application& app);

float& get_delta_time();
Window& get_window();
Renderer& get_renderer();
Camera& get_camera();
Scene& get_scene();

// ** PRIVATE **

inline std::unique_ptr<Window> _window;
inline std::unique_ptr<Renderer> _renderer;
inline Application* _app;
inline Camera _camera;
inline Scene _scene;

inline float _delta_time = 0;
inline float _last_frame_time = 0;
inline bool _show_default_imgui_window = true;

void init_glfw();
void init_window(uint width, uint height, const std::string& title);
void init_imgui();
void init_input_handler_callbacks();
void default_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

void poll_input();
void show_default_imgui_window();
void imgui_new_frame();
void imgui_render();
void clear_screen();
void update_delta_time();

void init();
void update();
void cleanup();

};

class Application {
public:
    Window& window = engine::get_window();
    Renderer& renderer = engine::get_renderer();
    float& delta_time = engine::get_delta_time();
    Camera& camera = engine::get_camera();
    Scene& scene = engine::get_scene();

    Application() {}
    ~Application() {}

    virtual void init() {}
    virtual void update() {}
    // NOTE: unload any textures you allocate here
    virtual void cleanup() {}
};

