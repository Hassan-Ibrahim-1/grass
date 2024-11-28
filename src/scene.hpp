#pragma once

#include <vector>
#include "common.hpp"
#include "game_object.hpp"
#include "material.hpp"
#include "transform.hpp"
#include "light.hpp"

// NOTE: remember to update shaders as well when you do anything to this
#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS  4
#define MAX_DIR_LIGHTS   4

class Scene {
public:
    ~Scene();

    // TODO: make this private
    std::vector<GameObject*> game_objects;
    std::array<PointLight*, MAX_POINT_LIGHTS> point_lights;
    std::array<SpotLight*, MAX_SPOT_LIGHTS> spot_lights;
    std::array<DirLight*, MAX_DIR_LIGHTS> directional_lights;

    // NOTE: make sure whenever you call add_x that whatever thats being added 
    // is heap allocated

    // game_object must be allocated on the heap
    // it will get freed in the destructor of Scene
    // when calling one of these functions, if the GameObject already exists in the scene
    // nothing will happen, it'll just get logged
    GameObject& create_game_object(Transform transform = Transform(), Material material = Material());
    void add_game_object(GameObject* game_object);
    // Not const because it does some stuff with setting the mesh vao
    void add_primitive(Cube* cube);
    void add_primitive(Rect* rect);
    void add_primitive(Circle* circle);
    void add_primitive(Sphere* sphere);

    uint game_object_count() const {
        return game_objects.size();
    }

    // NOTE: this frees the game object
    // Make sure this gameobject has been added to scene before
    // otherwise nothing will happen
    // DO NOT USE THE GAME OBJECT AFTER CALLING THIS
    // Returns nullptr
    GameObject* delete_game_object(GameObject* gobj);

    PointLight& create_point_light();
    SpotLight& create_spot_light();
    DirLight& create_dir_light();

    void add_point_light(PointLight* point_light);
    void add_spot_light(SpotLight* spot_light);
    void add_directional_light(DirLight* dir_light);

    size_t point_lights_used() const;
    size_t spot_lights_used() const;
    size_t dir_lights_used() const;

    bool has_lights() const;

    void clear_game_objects();
    void clear_lights();

private:
    // How lights are in use
    size_t _n_point_lights = 0;
    size_t _n_spot_lights = 0;
    size_t _n_dir_lights = 0;

    // NOTE: super simple rn. just increments a counter and returns the result
    uint generate_id();
    // returns -1 if no index is found
    int get_game_object_index(int obj_id);
};

