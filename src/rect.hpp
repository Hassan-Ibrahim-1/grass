#pragma once

#include "debug.hpp"
#include "game_object.hpp"
#include "material.hpp"
#include "transform.hpp"

struct Rect : public GameObject {
    static constexpr DrawCommand rect_draw_command = {
        DrawCommandType::DRAW_ELEMENTS,
        DrawCommandMode::TRIANGLES,
        6 // 6 indices
    };
    // NOTE: ignore scale.z it does nothing
    // Maybe add a transform2D class for this?

    /*Transform transform = Transform();*/
    /*Material material;*/
    /*bool hidden = false;*/

    Rect() {
        Mesh& mesh = create_mesh();
        mesh.draw_command = rect_draw_command;
    }
    Rect(Transform transform, Material material = Material())
        : GameObject(transform, material) {
        Mesh& mesh = create_mesh();
        mesh.draw_command = rect_draw_command;
    }
    Rect(glm::vec3 position, Material material = Material())
        : GameObject(Transform(position), material) {
        Mesh& mesh = create_mesh();
        mesh.draw_command = rect_draw_command;
    }

    void set_fill(bool fill) {
        ASSERT(!meshes.empty(), "rect mesh has not been created");
        if (fill) {
            Mesh& mesh = meshes.front();
            mesh.draw_command = rect_draw_command;
        }
        else {
            Mesh& mesh = meshes.front();
            mesh.draw_command.mode = DrawCommandMode::LINE_LOOP;
            mesh.draw_command.count = 4;
            mesh.draw_command.type = DrawCommandType::DRAW_ARRAYS;

        }
    }

    bool is_filled() {
        ASSERT(!meshes.empty(), "rect mesh has not been created");
        // HACK: this is really dumb. probably should have an internal bool
        // just checks if the count equals the default count
        // it won't equal the default count if it isn't filled
        return meshes.front().draw_command.count == rect_draw_command.count;
    }
};

