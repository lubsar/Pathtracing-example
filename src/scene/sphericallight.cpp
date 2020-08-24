#include "sphericallight.h"

namespace scene {
    SphericalLight::SphericalLight(const glm::vec3& position, const glm::vec3& color, float intensity) :
        m_position(position), m_color(color), m_intensity(intensity)
    {}

    glm::vec3& SphericalLight::GetColor() {
        return m_color;
    }

    glm::vec3& SphericalLight::GetPosition() {
        return m_position;
    }

    float SphericalLight::GetIntensity() {
        return  m_intensity;
    }
}


