#pragma once

#include <glm/glm.hpp>
#include "color.hpp"
#include "shader.hpp"

class Light {
public:
    Color ambient = Color(255);
    Color diffuse = Color(255);
    Color specular = Color(255);

    // name refers to the name of the light variable.
    void send_to_shader(const std::string& name, Shader& shader);
};

class PointLight : public Light {
public:
    glm::vec3 position = glm::vec3(0);

    float constant = 1;
    float linear = 0.893;
    float quadratic = 0.032;

    bool hidden = false;

    PointLight() = default;

    void send_to_shader(const std::string& name, Shader& shader);
};

class SpotLight : public Light {
public:
    glm::vec3 position = glm::vec3(0);
    glm::vec3 direction = glm::vec3(0);

    float constant = 1;
    float linear = 1;
    float quadratic = 1;

    float inner_cutoff = 0.0f;
    float outer_cutoff = 0.0f;
    bool hidden = false;

    SpotLight() = default;

    void send_to_shader(const std::string& name, Shader& shader);
};

class DirLight : public Light {
public:
    glm::vec3 direction = glm::vec3(0);

    DirLight() = default;

    void send_to_shader(const std::string& name, Shader& shader);
};

