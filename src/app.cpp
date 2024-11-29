#include <glad/glad.h>
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
    /*ground.hidden = true;*/
    scene.add_primitive(&ground);

    light.constant = 0.659;
    light.linear = 0.459;
    light.quadratic = 0.001;
    light.position.y = 13;
    scene.add_point_light(&light);

    create_random_grass();
}

void App::update() {
    /*update_grass();*/

    if (engine::cursor_enabled) {
        ImGui::Begin("scene");
        utils::imgui_cube("ground", ground);
        ImGui::Spacing();
        utils::imgui_point_light("light", light);
        ImGui::End();

        ImGui::Begin("grass");
        ImGui::DragFloat("mult", &mult, 0.001);
        /*for (size_t i = 0; i < grass.size(); i++) {*/
        /*    utils::imgui_transform("grass " + std::to_string(i), grass[i]->transform);*/
        /*}*/
        ImGui::End();
    }
}

void App::cleanup() {

}

GameObject& App::create_grass_blade() {
    GameObject& blade = scene.create_game_object();
    grass.emplace_back(&blade);
    auto& mesh = blade.create_mesh();
    /*scene.add_game_object(&obj);*/
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
    blade.transform.scale.x = 0.1;
    blade.material.color = Color(0, 255, 141);
    return blade;
}

void App::update_grass() {
    float offset = mult * cos(glfwGetTime());
    for (auto blade : grass) {
        auto& mesh = blade->meshes.front();
        /*float random = 0;*/
        float random = utils::random_float(-0.01, 0.01);
        mesh.vertices[0].position.x += offset + random;
        mesh.vertices[3].position.x += offset + random;
        /*mesh.vertices[4].position.x += offset;*/
        mesh.vertices[4].position.x = (mesh.vertices[3].position.x + mesh.vertices[0].position.x);
        /*mesh.vertices[3].position.z += offset;*/
        /*mesh.vertices[0].position.z += offset;*/

        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo());
        glBufferSubData(GL_ARRAY_BUFFER, 0, mesh.vertices.size() * sizeof(Vertex), mesh.vertices.data());

        /*mesh.delete_buffers();*/
        /*mesh.create_buffers();*/
    }
}

void App::create_random_grass() {
    for (uint i = 0; i < ngrass; i++) {
        auto& blade = create_grass_blade();
        blade.transform.position = random_point_on_ground();
        blade.transform.rotation.yaw = utils::random_float(0, 10);
    }
}

glm::vec3 App::random_point_on_ground() {
    glm::vec3 p;
    float start_x = ground.transform.position.x - (ground.transform.scale.x / 2);
    float start_z = ground.transform.position.z + (ground.transform.scale.z / 2);

    float end_x = ground.transform.position.x + (ground.transform.scale.x / 2);
    float end_z = ground.transform.position.z - (ground.transform.scale.z / 2);

    static constexpr float constraint = 2;

    start_x /= constraint;
    end_x /= constraint;
    start_z /= constraint;
    end_z /= constraint;

    p.x = utils::random_float(start_x, end_x);
    p.y = 1;
    p.z = utils::random_float(start_z, end_z);

    return p;
}

