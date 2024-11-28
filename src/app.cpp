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
        },
        {
            glm::vec3(0, 1, 0),
            glm::vec3(0, 0, 1),
            glm::vec2(0, 1)
        }
    };
    mesh.indices = {
        0, 1, 3,
        1, 2, 3,
        0, 3, 4
    };
    mesh.draw_command.mode = DrawCommandMode::TRIANGLES;
    mesh.draw_command.type = DrawCommandType::DRAW_ELEMENTS;
    mesh.create_buffers();
    grass.transform.scale.x = 0.1;
    grass.material.color = Color(0, 255, 141);
}

void App::update() {
    float offset = 0.001 * cos(glfwGetTime());
    auto& mesh = grass.meshes.front();
    mesh.vertices[4].position.x += offset;
    mesh.vertices[3].position.x += offset;
    mesh.vertices[0].position.x += offset;
    /*mesh.vertices[4].position.z += offset;*/
    /*mesh.vertices[3].position.z += offset;*/
    /*mesh.vertices[0].position.z += offset;*/
    mesh.delete_buffers();
    mesh.create_buffers();

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

