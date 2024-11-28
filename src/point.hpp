#pragma once

#include "color.hpp"
#include <glm/glm.hpp>

struct Point {
    glm::vec3 position = glm::vec3(0);
    glm::vec4 color = glm::vec4(1);
    bool hidden = false;
    
    Point() {}
    // NOTE: not using Color because of the way line vaos are handled
    Point(const glm::vec3& position = glm::vec3(0), const glm::vec4& color = glm::vec4(1));
    /*Point(const glm::vec3& position = glm::vec3(0), const Color& color = Color(255, 255, 255));*/
    glm::mat4 mat4();
};

