#ifndef MESH_H
#define MESH_H

#include <vector>
#include <initializer_list>

namespace scene {
    struct Vertex {
        float x, y, z, r, g, b;
    };

    class Mesh
    {
    private:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        int m_elementType = 0x04;
        bool m_isWireFrame = false;

    public:
        Mesh();

        unsigned int AddVertex(Vertex vertex);
        void AddIndices(std::initializer_list<unsigned int> indicies);

        void Clear();

        float* GetVertexData();
        unsigned int* GetIndexData();

        int LenVertices();
        int LenIndices();

        int ElementType();
        void SetElementType(int elementType);
    };
}

#endif // MESH_H
