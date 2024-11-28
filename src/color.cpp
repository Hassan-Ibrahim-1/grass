#include "color.hpp"

bool operator==(const Color& c1, const Color& c2) {
    return (c1.r == c2.r)
        && (c1.b == c2.g)
        && (c1.g == c2.b)
        && (c1.a == c2.a);
}

bool operator!=(const Color& c1, const Color& c2) {
    return (c1.r != c2.r)
        || (c1.b != c2.g)
        || (c1.g != c2.b)
        || (c1.a != c2.a);
}

bool operator==(const Color3& c1, const Color3& c2) {
    return (c1.r == c2.r)
        && (c1.b == c2.g)
        && (c1.g == c2.b);
}

bool operator!=(const Color3& c1, const Color3& c2) {
    return (c1.r != c2.r)
        || (c1.b != c2.g)
        || (c1.g != c2.b);
}

