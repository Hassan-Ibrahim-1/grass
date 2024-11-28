#include <cassert>
#include <cstddef>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include "renderer.hpp"
#include "debug.hpp"
#include "engine.hpp"
#include "fs.hpp"

Renderer::Renderer(Camera &main_camera, Scene &scene)
    : main_camera(&main_camera), main_scene(&scene) {
  generate_circle_vertices();
  init_models();
  init_vbos();
  init_vaos();
  init_shaders();
}

Renderer::~Renderer() {
    glDeleteBuffers(1, &_points_vbo);
    glDeleteBuffers(1, &_rects_vbo);
    glDeleteVertexArrays(1, &_points_vao);
    glDeleteVertexArrays(1, &_rects_vao);
}

void Renderer::draw_point(const Point& point) {
    _points.push_back(point);
}

void Renderer::draw_point(glm::vec3 position, Color color) {
    _points.push_back(Point(position, color.clamped_vec4()));
}

void Renderer::draw_line(Line& line) {
    _line_points.push_back(line.p1);
    _line_points.push_back(line.p2);
}

void Renderer::draw_line(Point p1, Point p2) {
    Line line(p1, p2);
    line.set_color(p1.color);
    _line_points.push_back(p1);
    _line_points.push_back(p2);
}

void Renderer::add_shader(Shader& shader) {
    _user_shaders.push_back(&shader);
}

void Renderer::reload_shaders() {
    LOG("Reloading shaders");

    /*shaders.rect.reload();*/
    /*shaders.circle.reload();*/
    /*shaders.cube.reload();*/
    /*shaders.sphere.reload();*/
    /**/
    /*shaders.textured_rect.reload();*/
    /*shaders.textured_cube.reload();*/
    /*shaders.textured_sphere.reload();*/

    shaders.point.reload();
    shaders.line.reload();
    shaders.basic_mesh.reload();
    shaders.basic_textured_mesh.reload();
    shaders.light_mesh.reload();
    shaders.light_textured_mesh.reload();
    shaders.depth.reload();

    for (Shader* shader : _user_shaders) {
        shader->reload();
    }
}

void Renderer::set_matrices(const glm::mat4& view, const glm::mat4& projection) {
    shaders.point.use();
    shaders.point.set_mat4("view", view);
    shaders.line.use();
    shaders.line.set_mat4("view", view);
    shaders.line.set_mat4("projection", projection);
    /**/
    /*shaders.rect.use();*/
    /*shaders.rect.set_mat4("view", view);*/
    /*shaders.rect.set_mat4("projection", projection);*/
    /**/
    /*shaders.circle.use();*/
    /*shaders.circle.set_mat4("view", view);*/
    /*shaders.circle.set_mat4("projection", projection);*/
    /**/
    /*shaders.cube.use();*/
    /*shaders.cube.set_mat4("view", view);*/
    /*shaders.cube.set_mat4("projection", projection);*/
    /**/
    /*shaders.sphere.use();*/
    /*shaders.sphere.set_mat4("view", view);*/
    /*shaders.sphere.set_mat4("projection", projection);*/
    /**/
    /*shaders.textured_rect.use();*/
    /*shaders.textured_rect.set_mat4("view", view);*/
    /*shaders.textured_rect.set_mat4("projection", projection);*/
    /**/
    /*shaders.textured_cube.use();*/
    /*shaders.textured_cube.set_mat4("view", view);*/
    /*shaders.textured_cube.set_mat4("projection", projection);*/
    /**/
    /*shaders.textured_sphere.use();*/
    /*shaders.textured_sphere.set_mat4("view", view);*/
    /*shaders.textured_sphere.set_mat4("projection", projection);*/
    /**/
    shaders.basic_mesh.use();
    shaders.basic_mesh.set_mat4("view", view);
    shaders.basic_mesh.set_mat4("projection", projection);

    shaders.basic_textured_mesh.use();
    shaders.basic_textured_mesh.set_mat4("view", view);
    shaders.basic_textured_mesh.set_mat4("projection", projection);

    shaders.light_mesh.use();
    shaders.light_mesh.set_mat4("view", view);
    shaders.light_mesh.set_mat4("projection", projection);

    shaders.light_textured_mesh.use();
    shaders.light_textured_mesh.set_mat4("view", view);
    shaders.light_textured_mesh.set_mat4("projection", projection);

    shaders.depth.use();
    shaders.depth.set_mat4("view", view);
    shaders.depth.set_mat4("projection", projection);

    for (auto shader : _user_shaders) {
        shader->use();
        shader->set_mat4("view", view);
        shader->set_mat4("projection", projection);
    }
}

void Renderer::render() {
    ASSERT(main_camera != nullptr, "Renderer::main_camera is a nullptr");
    ASSERT(main_scene != nullptr, "Renderer::main_scene is a nullptr");

    if (draw_as_hud) {
        set_matrices(glm::mat4(1), glm::mat4(1));
    }
    else {
        set_matrices(
            main_camera->get_view_matrix(),
            main_camera->get_perspective_matrix()
        );
    }

    update_vbos();

    if (depth_test_enabled) {
        glEnable(GL_DEPTH_TEST);
    }
    if (stencil_test_enabled) {
        glEnable(GL_STENCIL_TEST);
    }
    if (depth_view_enabled) {
        shaders.depth.set_float("near", main_camera->near);
        shaders.depth.set_float("far", main_camera->far);
    }
    if (wireframe_enabled) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // ** RENDER CALLS **

    render_points();
    render_lines();
    render_game_objects();
    render_lights();

    // Maybe use your own implementation of a dynamic array
    // clearing the array just sets the size to 0, capacity stays the same
    // push_back then adds to the 0th index and so on.
    // unless this is what std::vector already does?
    _points.clear();
    _line_points.clear();
}

uint Renderer::rect_vao() {
    return _rects_vao;
}
uint Renderer::circle_vao() {
    return _circles_vao;
}
uint Renderer::cube_vao() {
    return _cubes_vao;
}
uint Renderer::sphere_vao() {
    ASSERT(_sphere_model.loaded(), "Renderer::_sphere_model not loaded");
    return _sphere_model.meshes.front().vao();
}

const DrawCommand& Renderer::sphere_mesh_draw_command() {
    ASSERT(_sphere_model.loaded(), "Sphere model not loaded");
    return _sphere_model.meshes.front().draw_command;
}

void Renderer::render_points() {
    shaders.point.use();
    glBindVertexArray(_points_vao);
    glDrawArrays(GL_POINTS, 0, _points.size());
}

void Renderer::render_lines() {
    Shader* shader;
    if (depth_view_enabled) {
        shader = &shaders.depth;
    }
    else {
        shader = &shaders.line;
    }
    shader->use();
    glBindVertexArray(_lines_vao);
    glDrawArrays(GL_LINES, 0, _line_points.size());
}

void Renderer::render_game_objects() {
    for (GameObject* obj : main_scene->game_objects) {
        if (obj->hidden) {
            continue;
        }
        Shader* shader = nullptr;
        glm::mat4 model = obj->transform.get_mat4();
        // TODO: FIX THIS NESTING
        if (obj->material.shader) {
            shader = obj->material.shader.value();
            shader->use();
        }
        else {
            // Depth
            if (depth_view_enabled) {
                shader = &shaders.depth;
                shader->use();
            }
            // Textures
            else if (obj->material.has_diffuse_textures()) {
            // TODO: this probably isn't right - should check for other textures?
                if (!engine::get_scene().has_lights()) {
                    shader = &shaders.basic_textured_mesh;
                    shader->use();
                }
                // Only do lighting stuff if needed
                // Still do texture stuff afterwards tho
                else {
                    shader = &shaders.light_textured_mesh;
                    shader->use();

                    shader->set_float("material.shininess", obj->material.shininess);
                    shader->set_mat3(
                        "inverse_model",
                        glm::transpose(
                            glm::inverse(
                                model
                            )
                        )
                    );
                    send_light_data(*shader);
                }
                for (int i = 0; i < obj->material.diffuse_texture_count(); i++) {
                    // TODO: is this GL_TEXTURE0 and GL_TEXTURE1 stuff right?
                    // doesn't that just get overwritten on the iteration?
                    glActiveTexture(GL_TEXTURE0 + i);
                    shader->set_int(
                        "material.diffuse_texture" + std::to_string(i + 1),
                        GL_TEXTURE0 + i
                    );
                    obj->material.diffuse_textures[i].bind();
                }
                for (int i = 0; i < obj->material.specular_texture_count(); i++) {
                    // is this okay?
                    // loading both specular and diffuse textures with the same active texture
                    glActiveTexture(GL_TEXTURE0 + i);
                    shader->set_int(
                        "material.specular_texture" + std::to_string(i + 1),
                        GL_TEXTURE0 + i
                    );
                    obj->material.specular_textures[i].bind();
                }
            }
            // Textureless Lighting
            else {
                if (!engine::get_scene().has_lights()) {
                    shader = &shaders.basic_mesh;
                    shader->use();
                }
                else {
                    shader = &shaders.light_mesh;
                    shader->use();

                    shader->set_float("material.shininess", obj->material.shininess);
                    shader->set_mat3(
                        "inverse_model",
                        glm::transpose(
                            glm::inverse(
                                model
                            )
                        )
                    );
                    send_light_data(*shader);
                }
            }
        }
        shader->set_mat4("model", model);
        shader->set_vec3("material.color", obj->material.color.clamped_vec3());
        for (auto& mesh : obj->meshes) {
            render_mesh(mesh);
        }
    }
}

void Renderer::render_lights() {
    Scene& scene = engine::get_scene();
    Shader& shader = depth_view_enabled ? shaders.depth : shaders.basic_mesh;
    Transform sphere_transform;
    sphere_transform.scale = glm::vec3(0.1f);

    shader.use();

    // Point lights
    for (uint i = 0; i < scene.point_lights_used(); i++) {
        auto& light = *scene.point_lights[i];
        if (light.hidden) {
            continue;
        }
        sphere_transform.position = light.position;
        glm::mat4 model = sphere_transform.get_mat4();
        shader.set_mat4("model", model);
        shader.set_vec3("material.color", light.diffuse.clamped_vec3());
        render_mesh(_sphere_model.meshes.front());
    }

    // Spot lights
    Mesh square_pyramids_mesh;
    square_pyramids_mesh.set_vao(_square_pyramids_vao);
    square_pyramids_mesh.draw_command = {
        DrawCommandType::DRAW_ELEMENTS,
        DrawCommandMode::TRIANGLES,
        18
    };

    Transform square_pyramid_transform;
    square_pyramid_transform.scale = glm::vec3(0.2f);

    square_pyramid_transform.rotation.roll = 0;

    for (uint i = 0; i < scene.spot_lights_used(); i++) {
        auto& light = *scene.spot_lights[i];
        if (light.hidden) {
            continue;
        }
        square_pyramid_transform.position = light.position;

        glm::vec3 dir = glm::normalize(light.direction);
        square_pyramid_transform.rotation.yaw = glm::degrees(glm::atan(dir.x, dir.z));
        // HACK: -90 to make pitch look downward when direction is 0, -1, 0 and other stuff
        square_pyramid_transform.rotation.pitch = glm::degrees(glm::asin(-dir.y)) - 90;

        glm::mat4 model = square_pyramid_transform.get_mat4();
        shader.set_mat4("model", model);
        shader.set_vec3("material.color", light.diffuse.clamped_vec3());
        render_mesh(square_pyramids_mesh);
    }
}

void Renderer::render_mesh(const Mesh& mesh) {
    glBindVertexArray(mesh.vao());
    uint mode = draw_command_utils::draw_command_mode_to_gl_mode(mesh.draw_command.mode);
    if (mesh.draw_command.type == DrawCommandType::DRAW_ARRAYS) {
        glDrawArrays(mode, 0, mesh.draw_command.count);
    }
    else if (mesh.draw_command.type == DrawCommandType::DRAW_ELEMENTS) {
        glDrawElements(mode, mesh.draw_command.count, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}

void Renderer::init_models() {
    if (!_sphere_model.loaded()) {
        _sphere_model.load("models/sphere/sphere.obj");
    }
}

void Renderer::init_vbos() {
    glGenBuffers(1, &_points_vbo);
    glGenBuffers(1, &_rects_vbo);
    glGenBuffers(1, &_rects_vbo);
    glGenBuffers(1, &_circles_vbo);
    glGenBuffers(1, &_lines_vbo);
    glGenBuffers(1, &_cubes_vbo);
    glGenBuffers(1, &_square_pyramids_vbo);
}

void Renderer::init_vaos() {
    // points
    glGenVertexArrays(1, &_points_vao);
    glBindVertexArray(_points_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * _points.size(), _points.data(), GL_DYNAMIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Point), (void*)0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Point), (void*)offsetof(Point, color));
    glEnableVertexAttribArray(1);

    // Rects
    glGenVertexArrays(1, &_rects_vao);
    glBindVertexArray(_rects_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _rects_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _rect_vertices.size(),
                 _rect_vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &_rects_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rects_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * _rect_indices.size(),
                 _rect_indices.data(), GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 8, (void*)0);
    glEnableVertexAttribArray(0);

    // Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(float) * 8, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    // Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(float) * 8, (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    // circles
    glGenVertexArrays(1, &_circles_vao);
    glBindVertexArray(_circles_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _circles_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _circle_vertices.size(), _circle_vertices.data(), GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    // NOTE: Circles don't send any normal or texture coordinates rn

    // Lines
    glGenVertexArrays(1, &_lines_vao);
    glBindVertexArray(_lines_vao);

    // data is sent every frame
    glBindBuffer(GL_ARRAY_BUFFER, _lines_vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Point), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Point), (void*)offsetof(Point, color));
    glEnableVertexAttribArray(1);

    // Cubes
    glGenVertexArrays(1, &_cubes_vao);
    glBindVertexArray(_cubes_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _cubes_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _cube_vertices.size(), _cube_vertices.data(), GL_STATIC_DRAW);

    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 8, (void*)0);
    glEnableVertexAttribArray(0);

    // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(float) * 8, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    // Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(float) * 8, (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    // Square pyramids
    glGenVertexArrays(1, &_square_pyramids_vao);
    glBindVertexArray(_square_pyramids_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _square_pyramids_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _square_pyramid_vertices.size(), _square_pyramid_vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &_square_pyramids_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _square_pyramids_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * _square_pyramid_indices.size(),
                 _square_pyramid_indices.data(), GL_STATIC_DRAW);
    
    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 6, (void*)0);
    glEnableVertexAttribArray(0);

    // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(float) * 6, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
}

void Renderer::update_vbos() {
    glBindBuffer(GL_ARRAY_BUFFER, _lines_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * _line_points.size(), _line_points.data(), GL_DYNAMIC_DRAW);
    glBindVertexArray(_points_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * _points.size(), _points.data(), GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
}

void Renderer::init_shaders() {
    /*shaders.rect.load(fs::shader_path("rect.vert"), fs::shader_path("rect.frag"));*/
    /*shaders.circle.load(fs::shader_path("circle.vert"), fs::shader_path("circle.frag"));*/
    /*shaders.cube.load(fs::shader_path("cube.vert"), fs::shader_path("cube.frag"));*/
    /*shaders.sphere.load(fs::shader_path("sphere.vert"), fs::shader_path("sphere.frag"));*/
    /**/
    /*shaders.textured_rect.load(fs::shader_path("textured_rect.vert"),*/
    /*                           fs::shader_path("textured_rect.frag"));*/
    /*shaders.textured_cube.load(fs::shader_path("textured_cube.vert"),*/
    /*                           fs::shader_path("textured_cube.frag"));*/
    /*shaders.textured_sphere.load(fs::shader_path("textured_sphere.vert"),*/
    /*                             fs::shader_path("textured_sphere.frag"));*/

    shaders.point.load(fs::shader_path("point.vert"), fs::shader_path("point.frag"));
    shaders.line.load(fs::shader_path("line.vert"), fs::shader_path("line.frag"));
    shaders.basic_mesh.load(fs::shader_path("basic_mesh.vert"), fs::shader_path("basic_mesh.frag"));
    shaders.basic_textured_mesh.load(
        fs::shader_path("basic_textured_mesh.vert"),
        fs::shader_path("basic_textured_mesh.frag")
    );
    shaders.light_mesh.load(fs::shader_path("light_mesh.vert"), fs::shader_path("light_mesh.frag"));
    shaders.light_textured_mesh.load(
        fs::shader_path("light_textured_mesh.vert"),
        fs::shader_path("light_textured_mesh.frag")
    );
    shaders.depth.load(fs::shader_path("depth.vert"), fs::shader_path("depth.frag"));
}

void Renderer::send_light_data(Shader& shader) {
    Scene& scene = engine::get_scene();
    shader.use();

    shader.set_uint("n_point_lights_used", scene.point_lights_used());
    shader.set_uint("n_spot_lights_used", scene.spot_lights_used());
    shader.set_uint("n_dir_lights_used", scene.dir_lights_used());
    shader.set_vec3("view_pos", main_camera->transform.position);

    for (size_t i = 0; i < scene.point_lights_used(); i++) {
        auto& light = scene.point_lights[i];
        std::string name = "point_lights[" + std::to_string(i) + "]";
        light->send_to_shader(name, shader);
    }
    for (size_t i = 0; i < scene.spot_lights_used(); i++) {
        auto& light = scene.spot_lights[i];
        std::string name = "spot_lights[" + std::to_string(i) + "]";
        light->send_to_shader(name, shader);
    }
    for (size_t i = 0; i < scene.dir_lights_used(); i++) {
        auto& light = scene.directional_lights[i];
        std::string name = "dir_lights[" + std::to_string(i) + "]";
        light->send_to_shader(name, shader);
    }
}

void Renderer::generate_circle_vertices() {
    static constexpr float two_pi = M_PI * 2;
    static constexpr float radius = 0.5f;

    for (size_t i = 0; i < _n_circle_segments; i++) {
        // x, y, z
        _circle_vertices.push_back(_circle_start.x + radius * cos((float) i / _n_circle_segments * two_pi));
        _circle_vertices.push_back(_circle_start.y + radius * sin((float) i / _n_circle_segments * two_pi));
        _circle_vertices.push_back(0);
    }
}

