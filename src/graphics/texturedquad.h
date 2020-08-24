#ifndef TEXTUREDQUAD_H
#define TEXTUREDQUAD_H

#include "glm.hpp"

namespace graphics {
    class TexturedQuad
    {
    private:
        float* m_vertices;
        unsigned int m_numVertices;

        unsigned int* m_indices;
        unsigned int m_numIndices;

        glm::mat4 transformation;

    public:
        TexturedQuad();
        ~TexturedQuad();

        void SetTransformation(glm::mat4x4& transform);
        glm::mat4x4& GetTransformation();

        void RotateX(float angleDeg);
        void RotateY(float angleDeg);
        void RotateZ(float angleDeg);
        void Scale(float scale);
        void SetAspectRatio(float aspectRatio);

        void MoveTo(glm::vec3 position);

        float* GetVertices();
        unsigned int GetVertexSize();

        unsigned int* GetIndicies();
    };
}


#endif // TEXTUREDQUAD_H
