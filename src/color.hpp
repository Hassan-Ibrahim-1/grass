#pragma once

#include <glm/glm.hpp>

// Gives me an error if I include common so thats why this is here
using u8 = std::uint8_t;

struct Color {
    u8 r, g, b, a;

    Color() {}
    Color(u8 r, u8 g, u8 b, u8 a = 255.0f)
        : r(r), g(g), b(b), a(a) {}

    Color(glm::vec3 vec) : Color(vec.r * 255, vec.g * 255, vec.b * 255) {}
    Color(glm::vec4 vec) : Color(vec.r * 255, vec.g * 255, vec.b * 255, vec.a * 255) {}
    Color(float r) : Color(r, r, r, 255) {}

    glm::vec4 clamped_vec4() const {
        return glm::vec4(
            (float) r / 255, (float) g / 255,
            (float) b / 255, (float) a / 255
        );
    }

    glm::vec3 clamped_vec3() const {
        return glm::vec3(
            (float) r / 255,
            (float) g / 255,
            (float) b / 255
        );
    }

    Color& operator=(const glm::vec3& vec) {
        r = vec.r * 255;
        g = vec.g * 255;
        b = vec.b * 255;
        return *this;
    }

    Color& operator=(const glm::vec4& vec) {
        r = vec.r * 255;
        g = vec.g * 255;
        b = vec.b * 255;
        a = vec.a * 255;
        return *this;
    }

    friend bool operator==(const Color& c1, const Color& c2);
    friend bool operator!=(const Color& c1, const Color& c2);
};

struct Color3 {
    u8 r, g, b;

    Color3() {}
    Color3(u8 r, u8 g, u8 b)
        : r(r), g(g), b(b) {}

    Color3(glm::vec3 vec) : Color3(vec.r, vec.g, vec.b) {}
    Color3(glm::vec4 vec) : Color3(vec.r, vec.g, vec.b) {}
    Color3(float r) : Color3(r, r, r) {}

    glm::vec4 clamped_vec4() const {
        return glm::vec4(
            (float) r / 255, (float) g / 255,
            (float) b / 255, 0
        );
    }

    glm::vec3 clamped_vec3() const {
        return glm::vec3(
            (float) r / 255,
            (float) g / 255,
            (float) b / 255
        );
    }

    Color3& operator=(const glm::vec3& vec) {
        r = vec.r * 255;
        g = vec.g * 255;
        b = vec.b * 255;
        return *this;
    }

    Color3& operator=(const glm::vec4& vec) {
        r = vec.r * 255;
        g = vec.g * 255;
        b = vec.b * 255;
        return *this;
    }

    friend bool operator==(const Color3& c1, const Color3& c2);
    friend bool operator!=(const Color3& c1, const Color3& c2);
};

