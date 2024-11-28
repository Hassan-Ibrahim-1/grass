#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cstddef>
#include <sstream>
#include "model.hpp"
#include "debug.hpp"
#include "engine.hpp"
#include "vertex.hpp"

Model::Model(const char* path) {
    load(path);
}

Model::~Model() {
    for (auto& texture : _loaded_textures) {
        texture.unload();
    }
}

void Model::draw(Shader& shader) {
    for (const auto& mesh : meshes) {
        // TODO: set textures here
        engine::get_renderer().render_mesh(mesh);
    }
}

void Model::load(const std::string& path) {
    ASSERT(!_loaded, "Can't load a model after it has already been loaded");
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::stringstream ss;
        ss << "Model Loading Error: " << importer.GetErrorString() << '\n';
        ERROR("%s", ss.str().c_str());
    }
    _dir = path.substr(0, path.find_last_of('/'));

    process_node(scene->mRootNode, scene);
    for (auto& mesh : meshes) {
        mesh.create_buffers();
    }
    _loaded = true;
}

bool Model::loaded() const {
    return _loaded;
}

void Model::process_node(aiNode* node, const aiScene* scene) {
    for (size_t i = 0; i < node->mNumMeshes; i++) {
        // Node contains indices of corresponding in scene->mMeshes
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(process_mesh(mesh, scene));
    }
    for (size_t i = 0; i < node->mNumChildren; i++) {
        process_node(node->mChildren[i], scene);
    }
}
Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<uint> indices;

    for (size_t i = 0; i < mesh->mNumVertices; i++) {
        glm::vec2 tex_coords(0);
        if (mesh->mTextureCoords[0]) {
            tex_coords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        vertices.emplace_back(
            glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
            glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
            tex_coords
        );
    }
    for (size_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (size_t  j = 0; j < face.mNumIndices; j++) {
            indices.emplace_back(face.mIndices[j]);
        }
    }
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture2D> diffuse_maps =
            load_material_textures(
                material,
                aiTextureType_DIFFUSE,
                TextureType::DIFFUSE
            );
        textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

        std::vector<Texture2D> specular_maps =
            load_material_textures(
                material,
                aiTextureType_SPECULAR,
                TextureType::SPECULAR
            );
        textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    }

    // FIXME: memory leak - loading texture twice - once when assign diffuse / specular map
    // and once again when passing in textures and creating a mesh instance
    //
    // might be fixed idk because of passing in as texture id
    //
    // TODO: implement textures
    return Mesh(vertices, indices);
    /*switch (textures.size()) {*/
    /*    case 0:*/
    /*        return Mesh(vertices, indices, {});*/
    /*    case 1:*/
    /*        return Mesh(vertices, indices, Material(Color(255), textures[0].ID, -1));*/
    /**/
    /*    // case 2:*/
    /*    default:*/
    /*        return Mesh(vertices, indices, Material(Color(255), textures[0].ID, textures[1].ID));*/
    /*}*/
}

std::vector<Texture2D> Model::load_material_textures(
    aiMaterial* mat,
    aiTextureType type,
    TextureType texture_type) {

    // NOTE: Assumes that textures are in the same directory as the model
    std::vector<Texture2D> textures;

    for (size_t i = 0; i < mat->GetTextureCount(type); i++) {
        aiString path;
        mat->GetTexture(type, i, &path);
        bool skip = false;
        for (auto& tex : _loaded_textures) {
            if (std::strcmp(tex.path().data(), path.C_Str())) {
                textures.push_back(tex);
                skip = true;
                break;
            }
        }
        if (skip) continue;

        textures.emplace_back(_dir + "/" + path.C_Str(), texture_type);
        _loaded_textures.emplace_back(_dir + "/" + path.C_Str(), texture_type);
    }

    return textures;
}

