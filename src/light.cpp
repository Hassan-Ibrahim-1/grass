#include <glm/glm.hpp>
#include "light.hpp"

void Light::send_to_shader(const std::string& name, Shader& shader) {
    shader.use();
    shader.set_vec3(name + ".ambient", ambient.clamped_vec3());
    shader.set_vec3(name + ".diffuse", diffuse.clamped_vec3());
    shader.set_vec3(name + ".specular", specular.clamped_vec3());
}

void PointLight::send_to_shader(const std::string& name, Shader& shader)  {
    Light::send_to_shader(name, shader);
    shader.set_vec3(name + ".position", position);
    shader.set_float(name + ".constant", constant);
    shader.set_float(name + ".linear", linear);
    shader.set_float(name + ".quadratic", quadratic);
}

void SpotLight::send_to_shader(const std::string& name, Shader& shader) {
    Light::send_to_shader(name, shader);
    shader.set_vec3(name + ".position", position);
    shader.set_vec3(name + ".direction", direction);

    shader.set_float(name + ".constant", constant);
    shader.set_float(name + ".linear", linear);
    shader.set_float(name + ".quadratic", quadratic);

    shader.set_float(name + ".inner_cutoff", glm::cos(glm::radians(inner_cutoff)));
    shader.set_float(name + ".outer_cutoff", glm::cos(glm::radians(outer_cutoff)));
}

void DirLight::send_to_shader(const std::string& name, Shader& shader) {
    Light::send_to_shader(name, shader);
    shader.set_vec3(name + ".direction", direction);
}

