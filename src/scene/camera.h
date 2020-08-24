#ifndef CAMERA_H
#define CAMERA_H

#include "glm.hpp"

namespace scene {
    class Camera
    {
    private:
        glm::vec3 m_position;
        glm::mat4x4* m_view;
        glm::mat4x4* m_projection;

    public:
        Camera();

        float pitch;
        float yaw;
        float roll;

        float fov;
        float aspectRatio;
        float nearPlane;
        float farPlane;

        void Move(glm::vec3 offset);
        void Move(float xOffset, float yOffset, float zOffset);

        glm::vec3& GetPosition();

        glm::mat4x4& CalculateProjectionMatrix();
        glm::mat4x4& CalculateViewMatrix();
    };
}

#endif // CAMERA_H
