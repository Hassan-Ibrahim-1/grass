#include "game_object.hpp"

void GameObject::load_mesh_data(const Model& model) {
    for (const Mesh& model_mesh: model.meshes) {
        Mesh& mesh = create_mesh();
        mesh.set_vao(model_mesh.vao());
        mesh.draw_command = model_mesh.draw_command;
    }
}

