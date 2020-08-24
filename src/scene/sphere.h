#ifndef SPHERE_H
#define SPHERE_H


#include "glm.hpp"

namespace scene {
    class Sphere
    {
    private:
        glm::vec3 m_center;
        float m_radius;
        glm::vec3 m_color;

    public:
        Sphere(const glm::vec3& center, float radius, const glm::vec3& color);

        float GetRadius();
        glm::vec3& GetCenter();
        glm::vec3& GetColor();

        void Move(const glm::vec3& offset);
        void Move(float xOffset, float yOffset, float zOffset);
    };
}

#endif // SPHERE_H
