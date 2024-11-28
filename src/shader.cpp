#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <sstream>
#include <fstream>

#include <imgui.h>

#include "shader.hpp"
#include "debug.hpp"

Shader::Shader(const char* vertex_path, const char* fragment_path) {
    ID = glCreateProgram();
    _vertex_path = vertex_path;
    _fragment_path = fragment_path;
    load_shaders();
}

Shader::Shader(const std::string& vertex_path, const std::string& fragment_path)
    : Shader(vertex_path.c_str(), fragment_path.c_str()) {}

Shader::Shader() {
    ID = glCreateProgram();
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

bool Shader::check_shader_compilation_success(int shader) {
    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, _error);
        return false;
    }
    return true;
}

bool Shader::load_shader_from_path(const char* path, int flag) {
    int shader = glCreateShader(flag);
    std::string source = get_file_contents(path);
    const char* csrc = source.c_str();
    glShaderSource(shader, 1, &csrc, NULL);
    glCompileShader(shader);
    if (!check_shader_compilation_success(shader)) {
        return false;
    }
    glAttachShader(ID, shader);
    glDeleteShader(shader);
    return true;
}

void Shader::load_shaders() {
    ASSERT(load_shader_from_path(_vertex_path.c_str(), GL_VERTEX_SHADER), 
           "Bad vertex shader load at path: %s\n%s\n", _vertex_path.c_str(), _error);
    ASSERT(load_shader_from_path(_fragment_path.c_str(), GL_FRAGMENT_SHADER), 
           "Bad fragment shader load at path: %s\n%s\n", _fragment_path.c_str(), _error);
    glLinkProgram(ID);
    int success = 0;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    ASSERT(success, "Bad shader program link\n");
           
    _shader_loaded = true;
}

void Shader::load(const std::string& vertex_path, const std::string& fragment_path) {
    if (_shader_loaded) {
        LOG("WARNING: Shader already loaded: %s, %s\n", _vertex_path.c_str(), _fragment_path.c_str());
    }
    _vertex_path = vertex_path;
    _fragment_path = fragment_path;
    load_shaders();
}

void Shader::reload() {
    ASSERT(_shader_loaded,
           "Shader has to be loaded before it can be reloaded, path: %s, %s\n",
           _vertex_path.c_str(), _fragment_path.c_str());
    glDeleteProgram(ID);
    ID = glCreateProgram();
    load_shaders();
}

void Shader::use() {
    ASSERT(_shader_loaded,
           "Shader cannot be used before it is loaded, path: %s %s\n",
           _vertex_path.c_str(), _fragment_path.c_str());
    glUseProgram(ID);
}

const std::string Shader::get_error() const {
    return _error;
}

std::string Shader::get_file_contents(const char* path) {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(path);
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();
        return ss.str();
    }
    catch (std::ifstream::failure e) {
        std::stringstream ss;
        ss << "Bad " << path << " read: " << e.what() << "\ncode:\n" << e.code() << '\n';
        ERROR("%s", ss.str().c_str());
    }

    // Never reached
    return std::string();
}

void Shader::set_bool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value ? 1 : 0);
}

void Shader::set_int(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_uint(const std::string& name, uint value) const {
    glUniform1ui(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_float(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_vec3(const std::string& name, const glm::vec3& value) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::set_vec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::set_vec3(const std::string& name, const ImVec4& value) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::set_vec4(const std::string& name, const glm::vec4& value) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::set_mat3(const std::string& name, const glm::mat3& value) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_mat4(const std::string& name, const glm::mat4& value) const {
    // glm::value_ptr gets glsl compatible values
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_vec2(const std::string& name, const glm::vec2& value) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), value.x, value.y);
}
void Shader::set_vec2(const std::string& name, float x, float y) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

