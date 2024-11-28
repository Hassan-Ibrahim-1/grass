#pragma once

#include "game_object.hpp"
#include "transform.hpp"
#include "material.hpp"

struct Cube : GameObject {
    static constexpr DrawCommand cube_draw_command = {
        DrawCommandType::DRAW_ARRAYS, DrawCommandMode::TRIANGLES, 36
    };

    Cube() {
        Mesh& mesh = create_mesh();
        mesh.draw_command = cube_draw_command;
    }
    Cube(Transform transform, Material material)
        : GameObject(transform, material) {
        Mesh& mesh = create_mesh();
        mesh.draw_command = cube_draw_command;
    }
    Cube(glm::vec3 position, glm::vec3 scale = glm::vec3(1), Material material = Material())
        : GameObject(Transform(position, scale), material) {
        Mesh& mesh = create_mesh();
        mesh.draw_command = cube_draw_command;
    }


    /*Cube(Transform transform, Material mat)*/
    /*    : transform(transform), material(mat) {}*/
    /*Cube(glm::vec3 position, glm::vec3 scale = glm::vec3(1), Material material = Material())*/
    /*    : transform(position, scale), material(material) {}*/
};

