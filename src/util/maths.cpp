#include "maths.h"
#include "gtc/matrix_inverse.hpp"

namespace util {
    glm::mat4x4 rotationXMatrix(float angleDeg) {
        float angleRad = glm::radians(angleDeg);
        float cosa = glm::cos(angleRad);
        float sina = glm::sin(angleRad);

        return {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cosa, -sina, 0.0f,
            0.0f, sina, cosa, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    glm::mat4x4 rotationYMatrix(float angleDeg) {
        float angleRad = glm::radians(angleDeg);
        float cosa = glm::cos(angleRad);
        float sina = glm::sin(angleRad);

        return {
            cosa, 0.0f, sina, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -sina, 0.0f, cosa, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    glm::mat4x4 rotationZMatrix(float angleDeg) {
        float angleRad = glm::radians(angleDeg);
        float cosa = glm::cos(angleRad);
        float sina = glm::sin(angleRad);

        return {
            cosa, -sina, 0.0f, 0.0f,
            sina, cosa, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    glm::mat4x4 translationMatrix(const glm::vec3& translation) {
        return {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            translation.x, translation.y, translation.z, 1.0f
        };
    }

    glm::mat4x4 translationMatrix(float x, float y, float z) {
        return {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            x, y, z, 1.0f
        };
    }

    glm::mat4x4 inverse(glm::mat4x4& mat) {
        return glm::inverse(mat);
    }
}
