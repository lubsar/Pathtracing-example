#ifndef SPHERE_H
#define SPHERE_H


#include "glm.hpp"

namespace scene {
    class Sphere
    {
    public:
        glm::vec3 m_center;
        float m_radius;

        Sphere(glm::vec3 center, float radius);
    };
}

#endif // SPHERE_H
