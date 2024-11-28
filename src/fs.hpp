#pragma once

#include <string>

namespace fs {

inline std::string _shader_dir;
inline std::string _model_dir;

void init(const std::string& shader_dir, const std::string& model_dir);

// just provide the name of the shader with an extension
// to load shaders/circle.vert just provide circle.vert as an argument
std::string shader_path(const std::string& name);

// provide the name of the directory that contains the model
// this function will return a path to that directory
// ie to load models/backpack/ just provide backpack as an argument
// FIXME: figure out a better way to do this
std::string model_path(const std::string& name);

}

