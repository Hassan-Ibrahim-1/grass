#pragma once

#include "draw_command.hpp"
#include "game_object.hpp"
#include "transform.hpp"
#include "color.hpp"

struct Circle : GameObject {
    static constexpr DrawCommand circle_draw_command = {
        DrawCommandType::DRAW_ARRAYS,
        DrawCommandMode::TRIANGLE_FAN,
        152 // Renderer::_n_circle_segments + 2
    };

    Circle(Transform transform = Transform(), Material material = Material())
        : GameObject(transform, material) {
        auto& mesh = create_mesh();
        mesh.draw_command = circle_draw_command;
    }
    Circle(glm::vec3 position, Material material = Material())
        : GameObject(Transform(position), material) {
        auto& mesh = create_mesh();
        mesh.draw_command = circle_draw_command;
    }

    /*Circle(Transform transform = Transform(), Color color = Color(255, 255, 255));*/
    /*Circle(glm::vec3 position, Color color = Color(255, 255, 255));*/

    float radius();
    
    friend bool operator==(const Circle& c1, const Circle& c2);
    friend bool operator!=(const Circle& c1, const Circle& c2);
};

