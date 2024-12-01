#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "app.hpp"
#include "GLFW/glfw3.h"
#include "engine.hpp"
#include "utils.hpp"
#include "fs.hpp"

void App::init() {
    camera.velocity = 25;
    camera.transform.position = {0, 5, 7};

    ground.transform.scale = {
        70,
        1,
        70
    };
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

    renderer.add_shader(grass_shader);

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
    grass_mesh.create_buffers();

    create_random_grass();
    init_instance_vbo();

    grass_mesh.draw_command.mode = DrawCommandMode::TRIANGLES;
    grass_mesh.draw_command.type = DrawCommandType::DRAW_ELEMENTS_INSTANCED;
    grass_mesh.draw_command.vertex_count = grass_mesh.indices.size();
    grass_mesh.draw_command.instance_count = ngrass;
}

void App::update() {
    /*if (input::key_pressed(Key::O)) {*/
    /*    grass_shader.reload();*/
    /*}*/

    render_grass();

    if (engine::cursor_enabled) {
        ImGui::Begin("scene");
        utils::imgui_cube("ground", ground);
        ImGui::Spacing();
        utils::imgui_point_light("light", light);
        ImGui::End();
    }
}

void App::cleanup() {
}

glm::mat4& App::create_grass_blade() {
    grass_mats.emplace_back();
    return grass_mats.back();
}


void App::render_grass() {
    grass_shader.use();
    renderer.send_light_data(grass_shader);
    grass_shader.set_mat4("projection", camera.get_perspective_matrix());
    grass_shader.set_mat4("view", camera.get_view_matrix());
    grass_shader.set_float("time", glfwGetTime());
    grass_shader.set_vec3("material.color", grass_color.clamped_vec3());
    grass_shader.set_float("material.shininess", 32);
    renderer.render_mesh(grass_mesh);
}

void App::create_random_grass() {
    for (uint i = 0; i < ngrass; i++) {
        auto& model = create_grass_blade();
        Transform trans;
        trans.position = random_point_on_ground();
        trans.scale.x = 0.1f;
        trans.rotation.yaw = utils::random_float(0, 10);
        model = trans.get_mat4();
        grass_mats.emplace_back(utils::inverse_model(model));
    }
}

void App::init_instance_vbo() {
    glBindVertexArray(grass_mesh.vao());
    
    glGenBuffers(1, &instance_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, instance_vbo);
    // * 2 here because there are two matricies per blade
    glBufferData(GL_ARRAY_BUFFER, 2 * ngrass * sizeof(glm::mat4), &grass_mats[0], GL_STATIC_DRAW);

    auto v4s = sizeof(glm::vec4);

    // Model - binds to 3, 4, 5, 6
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 8*v4s, (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 8*v4s, (void*)(1*v4s));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 8*v4s, (void*)(2*v4s));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 8*v4s, (void*)(3*v4s));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    // Inverse - binds to 7, 8, 9, 10
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 8*v4s, (void*)(4*v4s));
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 8*v4s, (void*)(5*v4s));
    glEnableVertexAttribArray(9);
    glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, 8*v4s, (void*)(6*v4s));
    glEnableVertexAttribArray(10);
    glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, 8*v4s, (void*)(7*v4s));

    glVertexAttribDivisor(7, 1);
    glVertexAttribDivisor(8, 1);
    glVertexAttribDivisor(9, 1);
    glVertexAttribDivisor(10, 1);

    glBindVertexArray(0);
}

glm::vec3 App::random_point_on_ground() {
    glm::vec3 p;
    float start_x = ground.transform.position.x - (ground.transform.scale.x / 2);
    float start_z = ground.transform.position.z + (ground.transform.scale.z / 2);

    float end_x = ground.transform.position.x + (ground.transform.scale.x / 2);
    float end_z = ground.transform.position.z - (ground.transform.scale.z / 2);

    static constexpr float constraint = 1;

    start_x /= constraint;
    end_x /= constraint;
    start_z /= constraint;
    end_z /= constraint;

    p.x = utils::random_float(start_x, end_x);
    p.y = 1;
    p.z = utils::random_float(start_z, end_z);

    return p;
}

