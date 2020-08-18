#include "camera.h"
#include "util/maths.h"

namespace scene {
    Camera::Camera()
    {
        this->position = glm::vec3(0.0f);
        this->aspectRatio = 1.0f;
        this->yaw = 0.0f;
        this->pitch = 0.0f;
        this->roll = 0.0f;

        this->farPlane = 100.0f;
        this->nearPlane = 0.1f;

        this->view = new glm::mat4x4(1.0f);
        this->projection = new glm::mat4x4(1.0f);
    }

    void Camera::Move(glm::vec3 offset) {
        this->position += offset;
    }

    void Camera::Move(float xOffset, float yOffset, float zOffset) {
        this->position[0] += xOffset;
        this->position[1] += yOffset;
        this->position[2] += zOffset;
    }

    glm::mat4x4& Camera::CalculateViewMatrix() {
        using namespace util;

        *(this->view) = glm::mat4x4(1.0f) * rotationXMatrix(yaw)
                * rotationYMatrix(pitch) * rotationZMatrix(roll) * translationMatrix(-position);

        return *view;
    }

    glm::mat4x4& Camera::CalculateProjectionMatrix() {
        float y_scale = (1.0f/glm::tan(glm::radians(fov/2.0f))) * aspectRatio;
        float x_scale = y_scale / aspectRatio;
        float frustum_length = farPlane - nearPlane;

        (*this->projection) = {
            x_scale, 0.0f, 0.0f, 0.0f,
            0.0f, y_scale, 0.0f, 0.0f,
            0.0f, 0.0f, -((farPlane + nearPlane) / frustum_length), -1.0f,
            0.0f, 0.0f, -((2.0f*nearPlane * farPlane) / frustum_length), 0.0f
        };

        return *this->projection;
    }
}
