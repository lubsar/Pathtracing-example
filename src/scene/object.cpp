#include "object.h"
#include "util/maths.h"

using namespace util;

namespace scene {
    Object::Object()
    {
        this->transformation = glm::mat4x4(1.0f);
    }

    void Object::SetTransformation(glm::mat4x4 &transform) {
        this->transformation = transform;
    }

    glm::mat4x4 Object::GetTransformation() {
        return this->transformation;
    }

    void Object::RotateX(float angleDeg) {
        this->transformation *= rotationXMatrix(angleDeg);
    }

    void Object::RotateY(float angleDeg) {
        this->transformation *= rotationYMatrix(angleDeg);
    }

    void Object::RotateZ(float angleDeg) {
        this->transformation *= rotationZMatrix(angleDeg);
    }

    void Object::Scale(float scale) {
        this->transformation[0][0] *= scale;
        this->transformation[1][1] *= scale;
        this->transformation[2][2] *= scale;
    }

    void Object::Move(glm::vec3 offset) {
        this->transformation[3][0] += offset[0];
        this->transformation[3][1] += offset[1];
        this->transformation[3][2] += offset[2];
    }

    void Object::Move(float xOffset, float yOffset, float zOffset) {
        this->transformation[3][0] += xOffset;
        this->transformation[3][1] += yOffset;
        this->transformation[3][2] += zOffset;
    }

    Mesh& Object::GetMesh() {
        return mesh;
    }
}
