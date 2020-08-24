#ifndef SPHERICALLIGHT_H
#define SPHERICALLIGHT_H

#include "glm.hpp"

namespace scene {
    class SphericalLight
    {
    private:
        glm::vec3 m_position;
        glm::vec3 m_color;
        float m_intensity;
    public:
        SphericalLight(const glm::vec3& position, const glm::vec3& color, float intensity);

        glm::vec3& GetPosition();
        glm::vec3& GetColor();
        float GetIntensity();
    };
}


#endif // SPHERICALLIGHT_H
