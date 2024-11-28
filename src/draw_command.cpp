#include <glad/glad.h>
#include "draw_command.hpp"
#include "debug.hpp"

std::string draw_command_utils::to_string(DrawCommandMode mode) {
    switch (mode) {
        case DrawCommandMode::TRIANGLES: return "TRIANGLES";
        case DrawCommandMode::POINTS: return "POINTS";
        case DrawCommandMode::LINES: return "LINES";
        case DrawCommandMode::LINE_STRIP: return "LINE_STRIP";
        case DrawCommandMode::LINE_LOOP: return "LINE_LOOP";
        case DrawCommandMode::TRIANGLE_STRIP: return "TRIANGLE_STRIP";
        case DrawCommandMode::TRIANGLE_FAN: return "TRIANGLE_FAN";
        case DrawCommandMode::PATCHES: return "PATCHES";
        default: return "UNKNOWN";
    }
}

DrawCommandMode draw_command_utils::gl_mode_to_draw_command_mode(unsigned int gl_mode) {
    static const std::unordered_map<unsigned int, DrawCommandMode> mapping = {
        {GL_POINTS, DrawCommandMode::POINTS},           // GL_POINTS
        {GL_LINES, DrawCommandMode::LINES},            // GL_LINES
        {GL_TRIANGLES, DrawCommandMode::TRIANGLES},        // GL_TRIANGLES
        {GL_LINE_STRIP, DrawCommandMode::LINE_STRIP},       // GL_LINE_STRIP
        {GL_LINE_LOOP, DrawCommandMode::LINE_LOOP},        // GL_LINE_LOOP
        {GL_TRIANGLE_STRIP, DrawCommandMode::TRIANGLE_STRIP},   // GL_TRIANGLE_STRIP
        {GL_TRIANGLE_FAN, DrawCommandMode::TRIANGLE_FAN},     // GL_TRIANGLE_FAN
        {GL_PATCHES, DrawCommandMode::PATCHES},          // GL_PATCHES
    };

    auto it = mapping.find(gl_mode);
    if (it != mapping.end()) {
        return it->second;
    } else {
        ERROR("Invalid GL draw mode: %u", gl_mode);
    }
}

uint draw_command_utils::draw_command_mode_to_gl_mode(DrawCommandMode mode) {
    switch (mode) {
        case DrawCommandMode::POINTS: return GL_POINTS;
        case DrawCommandMode::LINES: return GL_LINES;
        case DrawCommandMode::TRIANGLES: return GL_TRIANGLES;
        case DrawCommandMode::LINE_STRIP: return GL_LINE_STRIP;
        case DrawCommandMode::LINE_LOOP: return GL_LINE_LOOP;
        case DrawCommandMode::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
        case DrawCommandMode::TRIANGLE_FAN: return GL_TRIANGLE_FAN;
        case DrawCommandMode::PATCHES: return GL_PATCHES;
        default: ERROR("Invalid DrawCommand: %s", draw_command_utils::to_string(mode).c_str());
    }
}

