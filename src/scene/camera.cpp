#include "camera.h"
#include "util/maths.h"

namespace scene {
    Camera::Camera()
    {
        this->m_position = glm::vec3(0.0f);
        this->aspectRatio = 1.0f;
        this->yaw = 0.0f;
        this->pitch = 0.0f;
        this->roll = 0.0f;

        this->farPlane = 100.0f;
        this->nearPlane = 0.1f;

        this->m_view = new glm::mat4x4(1.0f);
        this->m_projection = new glm::mat4x4(1.0f);
    }

    void Camera::Move(glm::vec3 offset) {
        this->m_position += offset;
    }

    void Camera::Move(float xOffset, float yOffset, float zOffset) {
        this->m_position[0] += xOffset;
        this->m_position[1] += yOffset;
        this->m_position[2] += zOffset;
    }

    glm::mat4x4& Camera::CalculateViewMatrix() {
        using namespace util;

        glm::mat4x4 tmp = translationMatrix(m_position) * ((rotationXMatrix(pitch) * rotationYMatrix(yaw)) * rotationZMatrix(roll));

        *this->m_view = inverse(tmp);

        return *this->m_view;
    }

    glm::mat4x4& Camera::CalculateProjectionMatrix() {
        float y_scale = (1.0f/glm::tan(glm::radians(fov/2.0f))) * aspectRatio;
        float x_scale = y_scale / aspectRatio;
        float frustum_length = farPlane - nearPlane;

        (*this->m_projection) = {
            x_scale, 0.0f, 0.0f, 0.0f,
            0.0f, y_scale, 0.0f, 0.0f,
            0.0f, 0.0f, -((farPlane + nearPlane) / frustum_length), -1.0f,
            0.0f, 0.0f, -((2.0f*nearPlane * farPlane) / frustum_length), 0.0f
        };

        return *this->m_projection;
    }

    glm::vec3& Camera::GetPosition() {
        return this->m_position;
    }
}
