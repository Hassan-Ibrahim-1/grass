#pragma once

#include <vector>
#include "color.hpp"
#include "texture2d.hpp"
#include "shader.hpp"

struct Material {
    Color color = Color(255, 255, 255);
    std::vector<Texture2D> diffuse_textures;
    std::vector<Texture2D> specular_textures;
    std::optional<Shader*> shader;
    float shininess = 32.0f;

    // TODO: emission

    Material() {}

    Material (Color color)
        : color(color) {}

    Material(Color color, const Texture2D& diffuse_texture, const Texture2D& specular_texture, Shader* shader = nullptr)
        : color(color),
          shader(shader) {
        diffuse_textures.emplace_back(diffuse_texture);
        specular_textures.emplace_back(specular_texture);
    }

    // NOTE: specular is not required here
    // FIXME: comparing uint with -1
    Material(Color color, uint diffuse_texture, uint specular_texture = -1, Shader* shader = nullptr)
        : color(color),
          shader(shader) {
        diffuse_textures.emplace_back(diffuse_texture, TextureType::DIFFUSE);
        auto spec_type = specular_texture != -1 ? TextureType::SPECULAR : TextureType::NONE;
        specular_textures.emplace_back(specular_texture, spec_type);
    }

    Texture2D& create_diffuse_texture() {
        diffuse_textures.emplace_back();
        return diffuse_textures.back();
    }
    Texture2D& create_specular_texture() {
        specular_textures.emplace_back();
        return specular_textures.back();
    }

    bool has_diffuse_textures() const {
        return !diffuse_textures.empty();
    }
    bool has_specular_textures() const {
        return !specular_textures.empty();
    }

    uint diffuse_texture_count() const {
        return diffuse_textures.size();
    }
    uint specular_texture_count() const {
        return specular_textures.size();
    }
};

