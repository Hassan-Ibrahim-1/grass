#include "debug.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>
#include <sstream>
#include "texture2d.hpp"

Texture2D::Texture2D(const std::string& path,
                    TextureType type,
                    bool default_texture_sampling)
    : _path(path), type(type) {
    stbi_set_flip_vertically_on_load(true);
    load(default_texture_sampling);
}

Texture2D::Texture2D(uint ID, TextureType type)
    : ID(ID), type(type) {
    if (type != TextureType::NONE) {
        _texture_loaded = true;
    }
}

Texture2D::Texture2D(TextureType type)
    : type(type) {
    stbi_set_flip_vertically_on_load(true);
}

Texture2D::~Texture2D() {

}

const std::string& Texture2D::path() {
    return _path;
}

void Texture2D::load(u8* data, bool default_texture_sampling) {
    if (_texture_loaded) return;

    glGenTextures(1, &ID);

    /*if (_data == nullptr) {*/
    /*    std::stringstream ss;*/
    /*    ss << "Data is null" << _path;*/
    /*    ErrorHandler::error(ss.str());*/
    /*    return;*/
    /*}*/

    GLenum format;
    switch(nr_channels) {
    case 1:
        format = GL_RED; break;
    case 3:
        format = GL_RGB; break;
    case 4:
        format = GL_RGBA; break;
    default:
        ERROR("Incompatible channel flag '%d' for path: %s\n", nr_channels, _path.c_str());
    }

    /*if (nr_channels == 1) {*/
    /*    format = GL_RED;*/
    /*}*/
    /*else if (nr_channels == 3) {*/
    /*    format = GL_RGB;*/
    /*}*/
    /*else if (nr_channels == 4) {*/
    /*    format = GL_RGBA;*/
    /*}*/
    /*else {*/
    /*    printf("nrChannels: %d\n", nr_channels);*/
    /*    ErrorHandler::error("Incompatible channel flag");*/
    /*    return;*/
    /*}*/

    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, _data);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (default_texture_sampling) {
        enable_default_texture_sampling();
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    _texture_loaded = true;
}

void Texture2D::load(bool default_texture_sampling) {
    if (_texture_loaded) return;

    glGenTextures(1, &ID);

    _data = stbi_load(_path.c_str(), &width, &height, &nr_channels, 0);
    ASSERT(_data != nullptr, "Bad texture load at path: %s", _path.c_str());

    GLenum format;
    switch(nr_channels) {
    case 1:
        format = GL_RED; break;
    case 3:
        format = GL_RGB; break;
    case 4:
        format = GL_RGBA; break;
    default:
        ERROR("Incompatible channel flag '%d' for path: %s\n", nr_channels, _path.c_str());
    }

    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, _data);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (default_texture_sampling) {
        enable_default_texture_sampling();
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(_data);
    _texture_loaded = true;
}

void Texture2D::unload() {
    if (_texture_loaded) {
        glDeleteTextures(1, &ID);
        _texture_loaded = false;
    }
}

void Texture2D::bind() const {
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture2D::enable_default_texture_sampling() {
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);*/
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);*/
    /**/
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);*/
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

    // NOTE: might be a problem?
    // Used for transparent textures. so that weird borders don't appear because of GL_REPEAT
    if (nr_channels == 4) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

