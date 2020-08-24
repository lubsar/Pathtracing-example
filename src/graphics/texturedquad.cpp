#include "texturedquad.h"

#include "util/maths.h"

namespace graphics {
    TexturedQuad::TexturedQuad()
    {
        this->transformation = glm::mat4x4(1.0f);

        this->m_vertices = new float[4*(3+2)]{
            -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 0.0f, 0.0f
        };

        this->m_indices = new unsigned int[6] {
          0, 1, 2,
          0, 3, 2
        };
    }

    TexturedQuad::~TexturedQuad() {
        delete[] m_vertices;
        delete[] m_indices;
    }

    unsigned int* TexturedQuad::GetIndicies() {
        return this->m_indices;
    }

    float* TexturedQuad::GetVertices() {
        return this->m_vertices;
    }

    unsigned int TexturedQuad::GetVertexSize() {
        return 5 * sizeof (float);
    }

    void TexturedQuad::SetTransformation(glm::mat4x4 &transform) {
        this->transformation = transform;
    }

    glm::mat4x4& TexturedQuad::GetTransformation() {
        return this->transformation;
    }

    void TexturedQuad::RotateX(float angleDeg) {
        this->transformation *= util::rotationXMatrix(angleDeg);
    }

    void TexturedQuad::RotateY(float angleDeg) {
        this->transformation *= util::rotationYMatrix(angleDeg);
    }

    void TexturedQuad::RotateZ(float angleDeg) {
        this->transformation *= util::rotationZMatrix(angleDeg);
    }

    void TexturedQuad::Scale(float scale) {
        this->transformation[0][0] *= scale;
        this->transformation[1][1] *= scale;
        this->transformation[2][2] *= scale;
    }

    void TexturedQuad::SetAspectRatio(float aspectRatio) {
        this->transformation[0][0] = 1.0f;
        this->transformation[1][1] = 1.0f/aspectRatio;
    }

    void TexturedQuad::MoveTo(glm::vec3 position) {
        this->transformation[3][0] = position.x;
        this->transformation[3][1] = position.y;
        this->transformation[3][2] = position.z;
    }
}
