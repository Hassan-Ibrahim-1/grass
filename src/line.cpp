#include "line.hpp"

/*Line::Line(const Point& p1, const Point& p2)*/
/*    : p1_position(p1.position), p1_color(p1.color),*/
/*      p2_position(p2.position), p2_color(p2.color) {}*/

Line::Line(const Point& p1, const Point& p2)
    : p1(p1), p2(p2) {}
void Line::set_color(const glm::vec4& color) {
    p1.color = color;
    p2.color = color;
    /*p1_color = color;*/
    /*p2_color = color;*/
}

