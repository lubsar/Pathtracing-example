#include "maths.h"

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
}
