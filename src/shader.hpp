#pragma once

#include "imgui.h"
#include <string>
#include <glm/glm.hpp>

using uint = unsigned int;

// Shader program
class Shader {
public:
    // -1 if the constructor failed
    unsigned int ID = 0;

    // build the shader
    Shader(const char* vertex_path, const char* fragment_path);
    Shader(const std::string& vertex_path, const std::string& fragment_path);
    // Set this to false if you don't want to create a program
    Shader();

    ~Shader();

    // to be used when shader was initialized using the default constructor
    // returns an error otherwise
    void load(const std::string& vertex_path, const std::string& fragment_path);

    // used for hotloading - shader has to be previously loaded for this to work
    void reload();

    // Activate the shader
    void use();

    // Functions for setting uniforms
    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, int value) const;
    void set_uint(const std::string& name, uint value) const;
    void set_float(const std::string& name, float value) const;
    void set_vec2(const std::string& name, const glm::vec2& value) const;
    void set_vec2(const std::string& name, float x, float y) const;
    void set_vec3(const std::string& name, const glm::vec3& value) const;
    void set_vec3(const std::string& name, float x, float y, float z) const;
    void set_vec3(const std::string& name, const ImVec4& value) const;
    void set_vec4(const std::string& name, const glm::vec4& value) const;
    void set_mat3(const std::string& name, const glm::mat3& value) const;
    void set_mat4(const std::string& name, const glm::mat4& value) const;

    const std::string get_error() const;

private:
    std::string _vertex_path;
    std::string _fragment_path;
    bool _shader_loaded = false;

    char _error[512];

    bool check_shader_compilation_success(int shader);
    bool load_shader_from_path(const char* path, int flag);
    void load_shaders();
    std::string get_file_contents(const char* path);
};

