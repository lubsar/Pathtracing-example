#ifndef OBJECT_H
#define OBJECT_H


#include "glm.hpp"
#include "mesh.h"

namespace scene {
    class Object
    {
    protected:
        glm::mat4x4 transformation;
        Mesh mesh;

    public:
        Object();

        void SetTransformation(glm::mat4x4& transform);
        glm::mat4x4& GetTransformation();

        void RotateX(float angleDeg);
        void RotateY(float angleDeg);
        void RotateZ(float angleDeg);
        void Scale(float scale);

        void Move(glm::vec3 offset);
        void Move(float xOffset, float yOffset, float zOffset);

        Mesh& GetMesh();
    };
}

#endif // OBJECT_H
