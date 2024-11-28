#pragma once

#include <glm/glm.hpp>

#include "point.hpp"

struct Line {
    Point p1;
    Point p2;
    bool hidden = false;

    Line(const Point& p1, const Point& p2);

    void set_color(const glm::vec4& color);
};

