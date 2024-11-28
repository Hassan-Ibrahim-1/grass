#pragma once

#include <glm/glm.hpp>
#include <string>
#include "camera.hpp"
#include "imgui.h"
#include "transform.hpp"
#include "common.hpp"
#include "color.hpp"
#include "circle.hpp"
#include "light.hpp"

namespace utils {

void init();


// TODO: clean all this up and move it to where it makes sense

glm::vec3 imvec4_to_glm_vec3(ImVec4& vec);
glm::vec4 imvec4_to_glm_vec4(ImVec4& vec);

ImVec4 glm_vec3_to_imvec4(glm::vec3& vec);

bool imgui_color_edit3(const std::string& name, Color& color);
bool imgui_color_edit3(const std::string& name, Color3& color);
bool imgui_color_edit4(const std::string& name, Color& color);
void imgui_fps_text();

// Primitives
void imgui_vec3(const std::string& name, glm::vec3 vec);
bool imgui_transform(const std::string& name, Transform& transform);
bool imgui_rect(const std::string& name, Rect& rect);
bool imgui_cube(const std::string& name, Cube& cube);
void imgui_sphere(const std::string& name, Sphere& sphere);
void imgui_circle(const std::string& name, Circle& circle);
void imgui_line(const std::string& name, Line& line);
bool imgui_game_object(const std::string& name, GameObject& obj);

// Lights
void imgui_light(const std::string& name, Light& light);
void imgui_point_light(const std::string& name, PointLight& light);
void imgui_spot_light(const std::string& name, SpotLight& light);
void imgui_dir_light(const std::string& name, DirLight& light);

float random_float(float min, float max);
float noise(int x, int y);

// TODO:
void add_to_imgui(Transform& transform);

bool mouse_in_circle(glm::vec3 position, float radius);
bool mouse_in_circle(Circle& circle);

bool point_in_rect(const Rect& rect, const glm::vec2& p);
bool mouse_in_rect(const Rect& rect);

// This is a horrible function
std::vector<u8> color3_vector_to_u8_vector(std::vector<Color3>& vec);

void write_jpeg(const char* filename, Color3* pixel_data, uint width, uint height);
void write_jpeg(const char* filename, u8* pixel_data, uint width, uint height);

void print_color(const Color& color);
void print_color(const Color3& color);

}

