#pragma once

#include <assimp/scene.h>
#include "mesh.hpp"
#include "shader.hpp"

class Model {
public:
    std::vector<Mesh> meshes;
    // TODO: why can't this just be _loaded_textures
    std::vector<Texture2D> textures;

    Model() {}
    Model(const char* path);
    ~Model();

    // TODO: get rid of this
    void draw(Shader& shader);
    // TODO: store textures here

    void load(const std::string& path);
    bool loaded() const;

private:
    std::vector<Texture2D> _loaded_textures;
    std::string _dir;
    bool _loaded = false;

    void process_node(aiNode* node, const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture2D> load_material_textures(
        aiMaterial* mat,
        aiTextureType type,
        TextureType texture_type
    );
};

