#include "cube.h"

namespace scene {
    Cube::Cube() :Object()
    {
        //1x1x1, centered around (0,0,0)
        this->mesh.AddVertex({-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f});
        this->mesh.AddVertex({0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f});
        this->mesh.AddVertex({0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f});
        this->mesh.AddVertex({-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f});
        this->mesh.AddVertex({-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f});
        this->mesh.AddVertex({0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f});
        this->mesh.AddVertex({0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f});
        this->mesh.AddVertex({-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f});

        this->mesh.AddIndices({0, 1, 2});
        this->mesh.AddIndices({0, 2, 3});
        this->mesh.AddIndices({0, 4, 5});
        this->mesh.AddIndices({0, 5, 1});
        this->mesh.AddIndices({1, 5, 6});
        this->mesh.AddIndices({1, 6, 2});
        this->mesh.AddIndices({2, 6, 7});
        this->mesh.AddIndices({2, 7, 3});
        this->mesh.AddIndices({3, 7, 4});
        this->mesh.AddIndices({3, 4, 0});
        this->mesh.AddIndices({4, 5, 6});
        this->mesh.AddIndices({4, 6, 7});
    }
}
