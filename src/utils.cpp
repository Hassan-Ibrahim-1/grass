#include <ctime>

#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "utils.hpp"
#include "engine.hpp"
#include "jpeglib.h"
#include "input.hpp"

void utils::init() {
    srand(static_cast<unsigned>(time(0)));
}

glm::vec3 utils::imvec4_to_glm_vec3(ImVec4& vec) {
    return glm::vec3(vec.x, vec.y, vec.z);
}

glm::vec4 utils::imvec4_to_glm_vec4(ImVec4& vec) {
    return glm::vec4(vec.x, vec.y, vec.z, vec.w);
}

ImVec4 utils::glm_vec3_to_imvec4(glm::vec3& vec) {
    return ImVec4(vec.x, vec.y, vec.z, 0);
}

bool utils::imgui_color_edit3(const std::string& name, Color& color) {
    glm::vec3 glm_col(color.clamped_vec3());
    bool ret = ImGui::ColorEdit3(name.c_str(), (float*)&glm_col);
    color = glm_col;
    return ret;
}

bool utils::imgui_color_edit3(const std::string& name, Color3& color) {
    glm::vec3 glm_col(color.clamped_vec3());
    bool ret = ImGui::ColorEdit3(name.c_str(), (float*)&glm_col);
    color = glm_col;
    return ret;
}

bool utils::imgui_color_edit4(const std::string& name, Color& color) {
    glm::vec4 glm_col(color.clamped_vec4());
    bool ret = ImGui::ColorEdit4(name.c_str(), (float*)&glm_col);
    color = glm_col;
    return ret;
}

void utils::imgui_fps_text() {
    ImGui::Text(
        "Application average %.3f ms/frame (%.1f FPS)",
        1000.0f / input::io->Framerate,
        input::io->Framerate);
}

void utils::imgui_vec3(const std::string& name, glm::vec3 vec) {
    ImGui::DragFloat3(name.c_str(), (float*)&vec);
}

bool utils::imgui_transform(const std::string& name, Transform& transform) {
    bool change_made = false;
    if (ImGui::DragFloat3((name + " pos").c_str(), (float*)&transform.position, 0.1)) {
        change_made = true;
    }
    if (ImGui::DragFloat3((name + " scale").c_str(), (float*)&transform.scale, 0.1)) {
        change_made = true;
    }
    if(ImGui::DragFloat3((name + " rotation").c_str(), (float*)&transform.rotation)) {
        change_made = true;
    }
    return change_made;
}

bool utils::imgui_rect(const std::string& name, Rect& rect) {
    bool edited = false;
    imgui_color_edit3(name + " color", rect.material.color);
    edited = imgui_transform(name, rect.transform);
    bool fill = rect.is_filled();
    ImGui::Checkbox("fill", &fill);
    rect.set_fill(fill);
    return edited;
}

bool utils::imgui_cube(const std::string& name, Cube& cube) {
    imgui_color_edit3(name + " color", cube.material.color);
    return imgui_transform(name, cube.transform);
}

void utils::imgui_sphere(const std::string& name, Sphere& sphere) {
    imgui_color_edit3(name + " color", sphere.material.color);
    imgui_transform(name, sphere.transform);
}

void utils::imgui_circle(const std::string& name, Circle& circle) {
    imgui_color_edit3(name + " color", circle.material.color);
    imgui_transform(name, circle.transform);
}

void utils::imgui_line(const std::string& name, Line& line) {
    ImGui::ColorEdit3((name + " p1 color").c_str(), (float*)&line.p1.color);
    ImGui::ColorEdit3((name + " p2 color").c_str(), (float*)&line.p2.color);
    ImGui::DragFloat3((name + "p1 pos").c_str(), (float*)&line.p1.position);
    ImGui::DragFloat3((name + "p2 pos").c_str(), (float*)&line.p2.position);
}

bool utils::imgui_game_object(const std::string& name, GameObject& obj) {
    bool transform_changed = false;
    imgui_color_edit3(name, obj.material.color);
    transform_changed = imgui_transform(name, obj.transform);
    ImGui::Checkbox((name + " hidden").c_str(), &obj.hidden);
    return transform_changed;
}

void utils::imgui_light(const std::string& name, Light& light) {
    imgui_color_edit3(name + " ambient", light.ambient);
    imgui_color_edit3(name + " diffuse", light.diffuse);
    imgui_color_edit3(name + " specular", light.specular);
}

void utils::imgui_point_light(const std::string& name, PointLight& light) {
    ImGui::DragFloat3((name + " pos").c_str(), (float*)&light.position, 0.1);

    imgui_light(name, light);

    ImGui::DragFloat((name + " const").c_str(), &light.constant, 0.001f);
    ImGui::DragFloat((name + " linear").c_str(), &light.linear, 0.001f);
    ImGui::DragFloat((name + " quad").c_str(), &light.quadratic, 0.001f);
}

void utils::imgui_spot_light(const std::string& name, SpotLight& light) {
    ImGui::DragFloat3((name + " pos").c_str(), (float*)&light.position, 0.1);
    ImGui::DragFloat3((name + " dir").c_str(), (float*)&light.direction, 0.01);

    imgui_light(name, light);

    ImGui::DragFloat((name + " const").c_str(), &light.constant, 0.001f);
    ImGui::DragFloat((name + " linear").c_str(), &light.linear, 0.001f);
    ImGui::DragFloat((name + " quad").c_str(), &light.quadratic, 0.001f);

    ImGui::DragFloat((name + " inner").c_str(), &light.inner_cutoff, 0.1);
    ImGui::DragFloat((name + " outer").c_str(), &light.outer_cutoff, 0.1);
}

void utils::imgui_dir_light(const std::string& name, DirLight& light) {
    ImGui::DragFloat3((name + " dir").c_str(), (float*)&light.direction, 0.01);
    imgui_light(name, light);
}

float utils::random_float(float min, float max) {
    return min + static_cast<float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
}

float utils::noise(int x, int y) {
    int n = x + y * 57;
    n = (n<<13) ^ n;
    return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589)
    & 0x7fffffff) / 1073741824.0); 
}

bool utils::mouse_in_circle(Circle& circle) {
    return mouse_in_circle(circle.transform.position, circle.radius());
}

bool utils::mouse_in_circle(glm::vec3 position, float radius) {
    glm::vec2 mouse_p = input::get_mouse_pos();
    float dist = 
          (mouse_p.x - position.x)
        * (mouse_p.x - position.x)
        + (mouse_p.y - position.y)
        * (mouse_p.y - position.y);
    return dist < radius * radius;
}

bool utils::mouse_in_rect(const Rect& rect) {
    auto mouse_p = input::get_mouse_pos();

    // Top left
    float start_x = rect.transform.position.x - (rect.transform.scale.x / 2);
    float start_y = rect.transform.position.y + (rect.transform.scale.y / 2);
    // HACK: this fixes rect y being bs
    start_y *= 2; 
    // Bottom right
    float end_x = rect.transform.position.x + (rect.transform.scale.x / 2);
    float end_y = rect.transform.position.y - (rect.transform.scale.y / 2);
    // HACK: this fixes rect y being bs
    end_y *= 2;

    return 
        (mouse_p.x > start_x && mouse_p.x < end_x)
     && (mouse_p.y < start_y && mouse_p.y > end_y);
}

bool utils::point_in_rect(const Rect& rect, const glm::vec2& p) {
    // NOTE: this function uses z values in place of y values
    // because thats what Grid needs when flat



    // Top left
    float start_x = rect.transform.position.x - (rect.transform.scale.x / 2);
    float start_y = rect.transform.position.z + (rect.transform.scale.z / 2);
    // HACK: this fixes rect y being bs
    /*start_y *= 2; */
    // Bottom right
    float end_x = rect.transform.position.x + (rect.transform.scale.x / 2);
    float end_y = rect.transform.position.z - (rect.transform.scale.z / 2);
    // HACK: this fixes rect y being bs
    /*end_y *= 2;*/

    return 
        (p.x > start_x && p.x < end_x)
     && (p.y < start_y && p.y > end_y);
}

std::vector<u8> utils::color3_vector_to_u8_vector(std::vector<Color3>& vec) {
    std::vector<u8> data;
    for (const auto& color : vec) {
        data.push_back(color.r);
        data.push_back(color.g);
        data.push_back(color.b);
    }
    return data;
}

void utils::write_jpeg(const char* filename, Color3* pixel_data, uint width, uint height) {
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    // Set up the error handler
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    // Open the output file
    FILE* outfile = fopen(filename, "wb");
    if (!outfile) {
        fprintf(stderr, "Could not open %s for writing\n", filename);
        return;
    }
    jpeg_stdio_dest(&cinfo, outfile);


    // Set image parameters
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3; // RGB
    cinfo.in_color_space = JCS_RGB;

    // Set default compression parameters
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 90, TRUE); // Quality from 0 to 100


    // Start compression
    jpeg_start_compress(&cinfo, TRUE);

    // Write the pixel data
    while (cinfo.next_scanline < cinfo.image_height) {
        JSAMPROW row_pointer[1]; // Pointer to a single row of pixels
        row_pointer[0] = (JSAMPROW)(&pixel_data[cinfo.next_scanline * width]);
        // Write a single scanline
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }


    // Finish compression
    jpeg_finish_compress(&cinfo);
    fclose(outfile);

    // Clean up
    jpeg_destroy_compress(&cinfo);
}

void utils::write_jpeg(const char* filename, u8* pixel_data, uint width, uint height) {
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    // Set up the error handler
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    // Open the output file
    FILE* outfile = fopen(filename, "wb");
    if (!outfile) {
        fprintf(stderr, "Could not open %s for writing\n", filename);
        return;
    }
    jpeg_stdio_dest(&cinfo, outfile);

    // Set image parameters
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3; // RGB
    cinfo.in_color_space = JCS_RGB;

    // Set default compression parameters
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 90, TRUE); // Quality from 0 to 100

    // Start compression
    jpeg_start_compress(&cinfo, TRUE);

    // Write the pixel data
    while (cinfo.next_scanline < cinfo.image_height) {
        unsigned char* row_pointer[3];
        row_pointer[0] = &pixel_data[cinfo.next_scanline * width * 3];
        jpeg_write_scanlines(&cinfo, row_pointer, 3);
    }

    // Finish compression
    jpeg_finish_compress(&cinfo);
    fclose(outfile);

    // Clean up
    jpeg_destroy_compress(&cinfo);
}

void utils::print_color(const Color& color) {
    printf("r: %d, g: %d, b: %d, a: %d\n", color.r, color.g, color.b, color.a);
}
void utils::print_color(const Color3& color) {
    printf("r: %d, g: %d, b: %d\n", color.r, color.g, color.b);
}

