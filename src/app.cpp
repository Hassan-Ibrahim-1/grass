#include "app.hpp"
#include "utils.hpp"

void App::init() {
    camera.velocity = 25;
    camera.transform.position = {0, 5, 7};

    ground.transform.scale = {
        30,
        1,
        30
    };
    ground.hidden = true;
    scene.add_primitive(&ground);

    light.constant = 0.659;
    light.linear = 0.459;
    light.quadratic = 0.001;
    light.position.y = 13;
    scene.add_point_light(&light);

    scene.add_game_object(&grass);
    auto& mesh = grass.create_mesh();
    mesh.vertices = {
        {
            glm::vec3(0.5f, 0.5f, 0.0f),
            glm::vec3(0, 0, 1),
            glm::vec2(1, 1)
        },
        {
            glm::vec3(0.5f, -0.5f, 0.0f),
            glm::vec3(0, 0, 1),
            glm::vec2(1, 0)
        },
        {
            glm::vec3(-0.5f, -0.5f, 0.0f),
            glm::vec3(0, 0, 1),
            glm::vec2(0, 0)

        },
        {
            glm::vec3(-0.5f, 0.5f, 0.0f),
            glm::vec3(0, 0, 1),
            glm::vec2(0, 1)
        }
    };
    mesh.indices = {
        0, 1, 3,
        1, 2, 3 
    };
    mesh.create_buffers();
}

void App::update() {
    if (engine::cursor_enabled) {
        ImGui::Begin("scene");
        utils::imgui_game_object("grass", grass);
        utils::imgui_cube("ground", ground);
        ImGui::Spacing();
        utils::imgui_point_light("light", light);
        ImGui::End();
    }
}

void App::cleanup() {

}

