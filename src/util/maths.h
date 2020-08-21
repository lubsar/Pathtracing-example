#ifndef MATHS_H
#define MATHS_H

#include "glm.hpp"

namespace util {
    glm::mat4x4 rotationXMatrix(float angleDeg);
    glm::mat4x4 rotationYMatrix(float angleDeg);
    glm::mat4x4 rotationZMatrix(float angleDeg);
    glm::mat4x4 translationMatrix(const glm::vec3& translation);
    glm::mat4x4 translationMatrix(float x, float y, float z);

    glm::mat4x4 inverse(glm::mat4x4& mat);

    int nextPowerOfTwo(int x);
}

#endif // MATHS_H
