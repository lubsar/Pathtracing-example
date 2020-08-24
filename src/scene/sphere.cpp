#include "sphere.h"
#include "iostream"

namespace scene {
    Sphere::Sphere(const glm::vec3& center, float radius, const glm::vec3& color) : m_center(center), m_radius(radius), m_color(color)
    {}

    void Sphere::Move(const glm::vec3& offset) {
        this->m_center += offset;
    }

    void Sphere::Move(float xOffset, float yOffset, float zOffset) {
        this->m_center.x += xOffset;
        this->m_center.x += yOffset;
        this->m_center.x += zOffset;
    }

    float Sphere::GetRadius() {
        return this->m_radius;
    }

    glm::vec3& Sphere::GetCenter() {
        return this->m_center;
    }

    glm::vec3& Sphere::GetColor() {
        return m_color;
    }
}
