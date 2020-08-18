#include "rasterizer.h"
#include "glslprogram.h"

using namespace scene;

namespace graphics {
    Rasterizer::Rasterizer() : Renderer()
    {
        initializeOpenGLFunctions();

        this->program = new GLSLProgram("shaders/test_vertex.vert", "shaders/test_fragment.frag");
    }

    Rasterizer::~Rasterizer()
    {

    }

    void Rasterizer::Init() {
        this->program->Prepare();

        //uniforms
        this->model_location = this->program->GetUniform("model");
        this->view_location = this->program->GetUniform("view");
        this->projection_location = this->program->GetUniform("projection");

        //TODO move somewhere else
        glEnable(GL_DEPTH_TEST);
    }

    void Rasterizer::Clear() {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Rasterizer::Draw(scene::Scene& scene, scene::Camera& camera) {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Rasterizer::Draw(scene::Object& obj, scene::Camera& camera) {
        Mesh mesh = obj.GetMesh();
        GLuint ebo, vbo;

        GLuint position_location, color_location, vao;

        // VAO
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // VBO
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh.LenVertices(), mesh.GetVertexData(), GL_STATIC_DRAW);

        // EBO - render order
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.LenIndices(), mesh.GetIndexData(), GL_STATIC_DRAW);

        //setting data layout
        position_location = this->program->GetAttribute("position");
        glEnableVertexAttribArray(position_location);
        glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

        color_location = this->program->GetAttribute("color");
        glEnableVertexAttribArray(color_location);
        glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(GLfloat)));

        // uniforms
        this->program->LoadUniformMat4x4(this->model_location, obj.GetTransformation());
        this->program->LoadUniformMat4x4(view_location, camera.CalculateViewMatrix());
        this->program->LoadUniformMat4x4(projection_location, camera.CalculateProjectionMatrix());

        // draw
        glDrawElements(GL_TRIANGLES, mesh.LenIndices(), GL_UNSIGNED_INT, 0);

        // cleanup
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);
    }
}
