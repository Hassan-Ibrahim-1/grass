#include <glm/gtc/matrix_transform.hpp>

#include "circle.hpp"
#include "engine.hpp"

/*Circle::Circle(Transform transform, Color color)*/
/*    : transform(transform), color(color) {}*/
/**/
/*Circle::Circle(glm::vec3 position, Color color)*/
/*    : Circle(Transform(position), color) {}*/

float Circle::radius() {
    // HACK: 0.5 is the base number used in Renderer::generate_circle_vertices()
    static constexpr float base_radius = 0.5f;

    glm::vec4 base_point_from_center(base_radius * cos(0), base_radius * sin(0), 0, 0);
    glm::mat4 scale_mat(1);
    scale_mat = glm::scale(scale_mat, transform.scale);
    // TODO: does this need projection matrix?
    glm::vec4 scaled_point = engine::get_camera().get_view_matrix() * scale_mat * base_point_from_center;
    // assume the circle is at 0, 0
    float radius = glm::distance(scaled_point,  glm::vec4(0));
    return abs(radius);
}

bool operator==(const Circle& c1, const Circle& c2) {
    return (c1.transform == c2.transform)
        && (c1.material.color == c2.material.color);
}
bool operator!=(const Circle& c1, const Circle& c2) {
    return (c1.transform != c2.transform)
        || (c1.material.color != c2.material.color);
}

