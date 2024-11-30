#include "debug.hpp"
#include "scene.hpp"
#include "engine.hpp"
#include "light.hpp"

Scene::~Scene() {
    clear_game_objects();
}

GameObject& Scene::create_game_object(Transform transform, Material material) {
    auto obj = game_objects.emplace_back(new GameObject(transform, material));
    obj->set_id(generate_id());
    return *obj;
}

void Scene::add_game_object(GameObject* game_object) {
    ASSERT(game_object != nullptr, "passing in game_object as a nullptr");
    // If the game object exists do nothing
    if (get_game_object_index(game_object->get_id()) != -1) {
        LOG("Game object with ID: %u already exists. Can't add it again.", game_object->get_id());
        return;
    }

    game_object->set_id(generate_id());
    game_objects.emplace_back(game_object);
}
void Scene::add_primitive(Cube* cube) {
    ASSERT(cube != nullptr, "passing in cube as a nullptr");
    if (get_game_object_index(cube->get_id()) != -1) {
        LOG("Game object with ID: %u already exists. Can't add it again.", cube->get_id());
        return;
    }

    /*ASSERT(cube->mesh_count() > 0, "Rect with ID: %u has no meshes", cube->get_id());*/
    cube->meshes.front().set_vao(engine::get_renderer().cube_vao());
    cube->set_id(generate_id());
    game_objects.emplace_back(cube);
}
void Scene::add_primitive(Rect* rect) {
    ASSERT(rect != nullptr, "passing in rect as a nullptr");
    if (get_game_object_index(rect->get_id()) != -1) {
        LOG("Game object with ID: %u already exists. Can't add it again.", rect->get_id());
        return;
    }

    rect->meshes.front().set_vao(engine::get_renderer().rect_vao());
    rect->set_id(generate_id());
    game_objects.emplace_back(rect);
}
void Scene::add_primitive(Circle* circle) {
    ASSERT(circle != nullptr, "passing in circle as a nullptr");

    if (get_game_object_index(circle->get_id()) != -1) {
        LOG("Game object with ID: %u already exists. Can't add it again.", circle->get_id());
        return;
    }

    circle->meshes.back().set_vao(engine::get_renderer().circle_vao());
    circle->set_id(generate_id());
    game_objects.emplace_back(circle);
}
void Scene::add_primitive(Sphere* sphere) {
    ASSERT(sphere != nullptr, "passing in sphere as a nullptr");

    if (get_game_object_index(sphere->get_id()) != -1) {
        LOG("Game object with ID: %u already exists. Can't add it again.", sphere->get_id());
        return;
    }

    sphere->meshes.back().set_vao(engine::get_renderer().sphere_vao());
    sphere->meshes.back().draw_command = engine::get_renderer().sphere_mesh_draw_command();
    sphere->set_id(generate_id());
    game_objects.emplace_back(sphere);
}

PointLight& Scene::create_point_light() {
    point_lights[_n_point_lights] = new PointLight();
    return *point_lights[_n_point_lights++];
}

SpotLight& Scene::create_spot_light() {
    spot_lights[_n_spot_lights] = new SpotLight();
    return *spot_lights[_n_spot_lights++];
}
DirLight& Scene::create_dir_light() {
    directional_lights[_n_dir_lights] = new DirLight();
    return *directional_lights[_n_dir_lights++];
}

GameObject* Scene::delete_game_object(GameObject* gobj) {
    /*if (game_objects.empty()) {*/
    /*    LOG("Trying to delete a game object when there are no game objects in the current scene.");*/
    /*    return gobj;*/
    /*}*/
    int index = get_game_object_index(gobj->get_id());
    ASSERT(index != -1, "Game object with id %u does not exist in the current scene", gobj->get_id());

    delete game_objects[index];
    game_objects.erase(game_objects.begin() + index);
    return nullptr;
}

void Scene::add_point_light(PointLight* point_light) {
    ASSERT(point_light != nullptr, "passing in point_light as a nullptr");
    point_lights[_n_point_lights] = point_light;
    _n_point_lights++;
}
void Scene::add_spot_light(SpotLight* spot_light) {
    ASSERT(spot_light != nullptr, "passing in spot_light as a nullptr");
    spot_lights[_n_spot_lights] = spot_light;
    _n_spot_lights++;
}
void Scene::add_directional_light(DirLight* dir_light) {
    ASSERT(dir_light != nullptr, "passing in dir_light as a nullptr");
    directional_lights[_n_dir_lights] = dir_light;
    _n_dir_lights++;
}

size_t Scene::point_lights_used() const {
    return _n_point_lights;
}
size_t Scene::spot_lights_used() const {
    return _n_spot_lights;
}
size_t Scene::dir_lights_used() const {
    return _n_dir_lights;
}

bool Scene::has_lights() const {
    return _n_point_lights > 0
        || _n_spot_lights > 0
        || _n_dir_lights > 0;
}

void Scene::clear_game_objects() {
    for (size_t i = 0; i < game_objects.size(); i++) {
        delete game_objects[i];
    }
}

void Scene::clear_lights() {
    for (size_t i  = 0; i < point_lights.size(); i++) {
        delete point_lights[i];
    }
    for (size_t i  = 0; i < spot_lights.size(); i++) {
        delete spot_lights[i];
    }
    for (size_t i  = 0; i < directional_lights.size(); i++) {
        delete directional_lights[i];
    }
}

uint Scene::generate_id() {
    static uint id = 0;
    return id++;
}

int Scene::get_game_object_index(int obj_id) {
    if (obj_id == -1) return -1;

    for (uint i = 0; i < game_objects.size(); i++) {
        if (game_objects[i]->get_id() == obj_id) {
            return i;
        }
    }
    return -1;
}

void Scene::set_skybox(const std::array<std::string, 6>& skybox_textures) {
    if (_skybox.loaded()) {
        _skybox.delete_textures();
    }
    _skybox.face_textures = skybox_textures;
    _skybox.load();
}

