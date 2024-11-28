#pragma once

#include <string>

using uint = unsigned int;
using u8 = std::uint8_t;

enum class TextureType {
    NONE,
    DIFFUSE,
    SPECULAR,
};

class Texture2D {
public:
    uint ID = 0;
    int width = 0;
    int height = 0;
    int nr_channels = 0;
    TextureType type;

    Texture2D() {}

    // automatically loads texture
    Texture2D(const std::string& path,
              TextureType type,
              bool default_texture_sampling = true);

    // store an already loaded texture
    Texture2D(uint ID, TextureType type);

    // TODO: implement default texture sampling here
    Texture2D(TextureType type);
    // For Already loaded textured

    // FIXME: THIS SHOULD NOT UNLOAD TEXTURES
    ~Texture2D(); // automatically unloads texture

    // Manually load the texture if you have your own data
    // Use the Texture2D(TextureType) constructor
    void load(u8* data, bool default_texture_sampling = true);

    const std::string& path();
    // manually unload a texture
    void unload();
    
    // Binds the texture in opengl
    void bind() const;

    // Enables some default texture sampling in opengl
    void enable_default_texture_sampling();

private:
    std::string _path;
    u8* _data;
    bool _texture_loaded = false;

    // automatically generates a mipmap
    void load(bool default_texture_sampling);
};

