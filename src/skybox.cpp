#include <glad/glad.h>
#include <stb_image.h>
#include "skybox.hpp"
#include "debug.hpp"
#include "common.hpp"

void Skybox::load() {
    if (_loaded) {
        LOG("skybox already loaded");
        return;
    }
    glGenTextures(1, &_texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _texture_id);

    int width, height, nr_channels;
    u8* data;
    stbi_set_flip_vertically_on_load(false);
    for (size_t i = 0; i < face_textures.size(); i++) {
        data = stbi_load(face_textures[i].c_str(), &width, &height, &nr_channels, 0);
        ASSERT(data, "bad file read: %s", face_textures[i].c_str());
        // + i makes it go through all the faces
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
                     height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // z-axis
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    _loaded = true;
}

void Skybox::delete_textures() {
    glDeleteTextures(1, &_texture_id);
    _loaded = false;
}

