#include <glm/gtc/matrix_transform.hpp>

#include "point.hpp"

Point::Point(const glm::vec3& position, const glm::vec4& color)
    : position(position), color(color) {}
/*Point::Point(const glm::vec3& position, const Color& color)*/
/*    : position(position), color(color) {}*/

glm::mat4 Point::mat4() {
    glm::mat4 mat(1);
    return glm::translate(mat, position);
}

