#include "mesh.h"
#include <stdarg.h>

#include <iostream>

namespace scene {
    struct Vertex vertex;

    Mesh::Mesh()
    {
        this->vertices = std::vector<Vertex>();
        this->indices = std::vector<unsigned int>();
    }

    unsigned int Mesh::AddVertex(Vertex vertex) {
        this->vertices.push_back(vertex);

        return vertices.size() - 1;
    }


    void Mesh::AddIndices(std::initializer_list<unsigned int> indicies) {
        for(unsigned int index : indicies) {
            this->indices.push_back(index);
        }
    }

    void Mesh::Clear() {
        this->indices.clear();
        this->vertices.clear();
    }

    float* Mesh::GetVertexData() {
        return reinterpret_cast<float*>(&this->vertices[0]);
    }

    unsigned int* Mesh::GetIndexData() {
        return &this->indices[0];
    }

    int Mesh::LenVertices() {
        return this->vertices.size();
    }

    int Mesh::LenIndices() {
        return this->indices.size();
    }

    int Mesh::ElementType() {
        return this->m_elementType;
    }

    void Mesh::SetElementType(int elementType) {
        this->m_elementType = elementType;
    }
}
