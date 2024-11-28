#pragma once

#include "model.hpp"
#include "transform.hpp"
#include "material.hpp"
#include "mesh.hpp"

class GameObject {
public:
    Transform transform;
    Material material;
    std::vector<Mesh> meshes;
    bool hidden = false;

    GameObject() {}
    GameObject(Transform transform, Material material = Material())
        : transform(transform), material(material) {}

    Mesh& create_mesh() {
        meshes.emplace_back();
        return meshes.back();
    }
    uint mesh_count() {
        return meshes.size();
    }

    // Just sets the meshes and draw commands
    // from a Model keep this. only sets vaos
    // doesn't copy mesh data. so don't delete the original
    // vao / mesh
    void load_mesh_data(const Model& model);

    void set_id(uint id) {
        _id = id;
    }
    uint get_id() {
        return _id;
    }

    // NOTE: Be carefull when using this
    // It just checks the value of the ids against each other, nothing else
    // Make sure to add to scene before using this
    friend bool operator==(const GameObject& g1, const GameObject& g2);
    friend bool operator!=(const GameObject& g1, const GameObject& g2);

private:
    int _id = -1;
};

inline bool operator==(const GameObject& g1, const GameObject& g2) {
    return g1._id == g2._id;
}

inline bool operator!=(const GameObject& g1, const GameObject& g2) {
    return g1._id != g2._id;
}

