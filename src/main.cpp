#include "engine.hpp"
#include "app.hpp"

int main() {
    EngineInitInfo cinfo;
    cinfo.window_width = 1280;
    cinfo.window_height = 720;
    cinfo.window_title = "App";
    cinfo.clear_color = Color(glm::vec3(0.1, 0.1, 0.1));
    cinfo.cursor_enabled = false;

    // Call init before creating an Application instance
    engine::init(cinfo);
    App app;
    engine::run(app);

    return 0;
}

