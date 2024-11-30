#pragma once

#include <string>

using uint = unsigned int;

enum class DrawCommandType {
    DRAW_ARRAYS,
    DRAW_ELEMENTS,
    DRAW_ARRAYS_INSTANCED,
    DRAW_ELEMENTS_INSTANCED,
};

enum class DrawCommandMode {
    TRIANGLES,
    POINTS,
    LINES,
    LINE_STRIP,
    LINE_LOOP,
    TRIANGLE_STRIP,
    TRIANGLE_FAN,
    PATCHES
};

namespace draw_command_utils {
    DrawCommandMode gl_mode_to_draw_command_mode(uint gl_mode);
    uint draw_command_mode_to_gl_mode(DrawCommandMode mode);
    std::string to_string(DrawCommandMode mode);
}

struct DrawCommand {
    DrawCommandType type;
    DrawCommandMode mode;
    size_t vertex_count = 0;
    uint instance_count = 0;
};

