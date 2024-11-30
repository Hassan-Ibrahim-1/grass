#include "game_object.hpp"

void GameObject::load_mesh_data(const std::vector<Mesh>& meshes) {
    for (const Mesh& m: meshes) {
        Mesh& mesh = create_mesh();
        mesh.set_vao(m.vao());
        mesh.draw_command = m.draw_command;
    }
}

void GameObject::load_texture_data(const std::vector<Texture2D>& textures) {
    for (auto& t : textures) {
        if (t.type == TextureType::DIFFUSE) {
            auto& texture = material.create_diffuse_texture();
            texture = Texture2D(t.ID, TextureType::DIFFUSE);
        }
        else if (t.type == TextureType::SPECULAR) {
            auto& texture = material.create_specular_texture();
            texture = Texture2D(t.ID, TextureType::SPECULAR);
        }

    }
}
void GameObject::load_model_data(const Model& model) {
    load_mesh_data(model.meshes);
    load_texture_data(model.textures);
}

