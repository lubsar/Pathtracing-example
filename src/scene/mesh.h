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
    public:
        Mesh();

        unsigned int AddVertex(Vertex vertex);
        void AddIndices(std::initializer_list<unsigned int> indicies);

        float* GetVertexData();
        unsigned int* GetIndexData();

        int LenVertices();
        int LenIndices();
    };
}

#endif // MESH_H
