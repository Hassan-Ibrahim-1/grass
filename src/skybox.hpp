#pragma once

#include <string>
#include <array>

using uint = unsigned int;

class Skybox {
public:
    Skybox() {}
    ~Skybox() {
        if (_loaded) {
            delete_textures();
        }
    }
    /*
     Provide in the following order:
     right
     left
     top
     bottom
     front
     back
    */
    std::array<std::string, 6> face_textures;
    void load();
    bool loaded() const { return _loaded; }

    uint get_texture_id() { return _texture_id; }
    void delete_textures();

private:
    uint _texture_id = 0;
    bool _loaded = false;
};

