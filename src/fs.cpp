#include <filesystem>
#include "fs.hpp"

void fs::init(const std::string& shader_dir, const std::string& model_dir) {
    _shader_dir = shader_dir;
    _model_dir = model_dir;
    if (_shader_dir.back() != '/') {
        _shader_dir = _shader_dir + "/";
    }
    if (_model_dir.back() != '/') {
        _model_dir = _model_dir + "/";
    }
}

std::string fs::shader_path(const std::string& name) {
    return _shader_dir + name;
}

std::string fs::model_path(const std::string& name) {
    return _model_dir + name;
}

