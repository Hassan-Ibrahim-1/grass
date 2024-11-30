#include <glad/glad.h>
#include "app.hpp"
#include "GLFW/glfw3.h"
#include "utils.hpp"
#include "fs.hpp"

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
    ground.hidden = false;

    light.constant = 0.659;
    light.linear = 0.459;
    light.quadratic = 0.001;
    light.position.y = 13;
    scene.add_point_light(&light);

    grass_shader.load(
        fs::shader_path("grass.vert"),
        fs::shader_path("light_mesh.frag")
    );

    grass_mesh.vertices = {
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
    grass_mesh.indices = {
        0, 1, 3,
        1, 2, 3,
        0, 3, 4
    };
    grass_mesh.draw_command.mode = DrawCommandMode::TRIANGLES;
    grass_mesh.draw_command.type = DrawCommandType::DRAW_ELEMENTS_INSTANCED;
    grass_mesh.draw_command.instance_count = ngrass;
    grass_mesh.create_buffers();

    create_random_grass();
    send_transform_data();
}

void App::update() {
    /*update_grass();*/

    render_grass();

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

Transform& App::create_grass_blade() {
    grass_transforms.emplace_back(Transform());
    return grass_transforms.back();
}

void App::render_grass() {
    grass_shader.use();
    renderer.send_light_data(grass_shader);
    grass_shader.set_float("time", glfwGetTime());
    grass_shader.set_vec3("material.color", grass_color.clamped_vec3());
    grass_shader.set_float("material.shininess", 32);
    renderer.render_mesh(grass_mesh);
}

void App::update_grass() {
    float offset = mult * cos(glfwGetTime());
    for (auto blade : grass_transforms) {
        float random = 0;
        /*float random = utils::random_float(-0.01, 0.01);*/
        // NOTE: top left, top right, top most are updated
        /*mesh.vertices[0].position.x += offset + random;*/
        /*mesh.vertices[3].position.x += offset + random;*/
        /*mesh.vertices[4].position.x += offset;*/
        /*mesh.vertices[4].position.x = (mesh.vertices[3].position.x + mesh.vertices[0].position.x);*/
        /*mesh.vertices[3].position.z += offset;*/
        /*mesh.vertices[0].position.z += offset;*/

        /*glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo());*/
        /*glBufferSubData(GL_ARRAY_BUFFER, 0, mesh.vertices.size() * sizeof(Vertex), mesh.vertices.data());*/

        /*mesh.delete_buffers();*/
        /*mesh.create_buffers();*/
    }
}

void App::create_random_grass() {
    for (uint i = 0; i < ngrass; i++) {
        auto& blade = create_grass_blade();
        blade.position = random_point_on_ground();
        blade.scale.x = 0.1f;
        /*blade.rotation.yaw = utils::random_float(0, 10);*/
    }
}

void App::send_transform_data() {
    grass_shader.use();
    for (uint i = 0; i < ngrass; i++) {
        auto model_name = "models[" + std::to_string(i) + "]";
        grass_shader.set_mat4(model_name, grass_transforms[i].get_mat4());
        auto inverse_model_name = "inverse_models[" + std::to_string(i) + "]";
        grass_shader.set_mat3(
            inverse_model_name,
            utils::inverse_model(grass_transforms[i].get_mat4())
        );
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

