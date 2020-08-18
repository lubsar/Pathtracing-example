#ifndef MATHS_H
#define MATHS_H

#include "glm.hpp"

namespace util {
    glm::mat4x4 rotationXMatrix(float angleDeg);
    glm::mat4x4 rotationYMatrix(float angleDeg);
    glm::mat4x4 rotationZMatrix(float angleDeg);
}

#endif // MATHS_H
