#include "renderer.h"

#include "util/shader.h"
#include "gtc/type_ptr.hpp"

#include "iostream"

using namespace scene;

namespace graphics {
    Renderer::Renderer() : m_sphere(MeshSphere(20, 20)), m_quad(TexturedQuad()) {
        initializeOpenGLFunctions();

        this->m_program = new GLSLProgram();
        this->m_programInst = new GLSLProgram();
        this->m_programTex = new GLSLProgram();

        // basic program
        std::string vertex_src = util::load_shader_source("shaders/renderer.vert");
        std::string fragment_src = util::load_shader_source("shaders/renderer.frag");

        this->m_program->AddShader(GL_VERTEX_SHADER, vertex_src);
        this->m_program->AddShader(GL_FRAGMENT_SHADER, fragment_src);

        this->m_program->BindFragDataLocation(0, "outColor");
        this->m_program->Link();

        //uniforms
        this->m_program->AddUniform("model");
        this->m_program->AddUniform("view");
        this->m_program->AddUniform("projection");

        // instanced program
        std::string vertex_src_inst = util::load_shader_source("shaders/renderer_instanced.vert");

        this->m_programInst->AddShader(GL_VERTEX_SHADER, vertex_src_inst);
        this->m_programInst->AddShader(GL_FRAGMENT_SHADER, fragment_src);

        this->m_programInst->BindFragDataLocation(0, "outColor");
        this->m_programInst->Link();

        //uniforms
        this->m_programInst->AddUniform("view");
        this->m_programInst->AddUniform("projection");

        // textured program
        std::string vertex_src_tex = util::load_shader_source("shaders/renderer_texture.vert");
        std::string fragment_src_tex  = util::load_shader_source("shaders/renderer_texture.frag");

        this->m_programTex->AddShader(GL_VERTEX_SHADER, vertex_src_tex);
        this->m_programTex->AddShader(GL_FRAGMENT_SHADER, fragment_src_tex);

        this->m_programTex->BindFragDataLocation(0, "outColor");
        this->m_programTex->Link();

        //uniforms
        this->m_programTex->AddUniform("model");
        this->m_programTex->AddUniform("view");
        this->m_programTex->AddUniform("projection");
    }

    Renderer::~Renderer() {
        delete m_program;
        delete m_programInst;
        delete m_programTex;
    }

    void Renderer::Prepare() {
        glEnable(GL_DEPTH_TEST);
    }

    void Renderer::Clear() {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::Draw(scene::Scene& scene, scene::Camera& camera) {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // "meshes"
        if(scene.GetObjects()->size() > 0) {
            this->m_program->Use();

            this->m_program->SetUniform("view", camera.CalculateViewMatrix());
            this->m_program->SetUniform("projection", camera.CalculateProjectionMatrix());

            for(Object* obj : *(scene.GetObjects())) {
              this->Draw(*obj);
            }
        }

        if(scene.GetSpheres()->size() > 0) {
            this->m_programInst->Use();

            this->m_programInst->SetUniform("view", camera.CalculateViewMatrix());
            this->m_programInst->SetUniform("projection", camera.CalculateProjectionMatrix());

            Draw(scene.GetSpheres());
        }
    }

    void Renderer::Draw(scene::Object& obj, scene::Camera& camera) {
        this->m_program->Use();

        // camera matrices
        this->m_program->SetUniform("view", camera.CalculateViewMatrix());
        this->m_program->SetUniform("projection", camera.CalculateProjectionMatrix());

        Draw(obj);
    }

    void Renderer::Draw(scene::Object& obj) {
        Mesh mesh = obj.GetMesh();
        GLuint vao, ebo, vbo;

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
        this->m_program->SetAttribute("position", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        this->m_program->SetAttribute("color", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(GLfloat)));

        // model matrix
        this->m_program->SetUniform("model", obj.GetTransformation());

        // draw
        glDrawElements(mesh.ElementType(), mesh.LenIndices(), GL_UNSIGNED_INT, 0);

        // cleanup
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);
    }

    void Renderer::Draw(std::vector<scene::Sphere*> *spheres) {
        Mesh mesh = this->m_sphere.GetMesh();
        GLuint vao, ebo, vbo, vbo_inst;

        // sphere instance data
        glm::vec4* data = new glm::vec4[spheres->size() *2];

        int counter = 0;
        for(scene::Sphere* sphere : *spheres) {
            data[counter++] = glm::vec4(sphere->GetCenter(), sphere->GetRadius());
            data[counter++] = glm::vec4(sphere->GetColor(), 1.0f);
        }

        // VAO
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // mesh VBO
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh.LenVertices(), mesh.GetVertexData(), GL_STATIC_DRAW);

        // instance data VBO
        glGenBuffers(1, &vbo_inst);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_inst);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 2 * spheres->size(), data, GL_STATIC_DRAW);

        // EBO - render order
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.LenIndices(), mesh.GetIndexData(), GL_STATIC_DRAW);

        // setting data layout
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        this->m_programInst->SetAttribute("position", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

        // instance atrib
        glBindBuffer(GL_ARRAY_BUFFER, vbo_inst);
        GLint atrib = this->m_programInst->SetAttribute("center", 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 2, 0);
        glVertexAttribDivisor(atrib, 1);
        atrib = this->m_programInst->SetAttribute("color", 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) *2, (void*)(sizeof(glm::vec4)));
        glVertexAttribDivisor(atrib, 1);

        // draw instances
        glDrawElementsInstanced(mesh.ElementType(), mesh.LenIndices(), GL_UNSIGNED_INT, 0, spheres->size());

        // cleanup
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &vbo_inst);
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);
    }

    void Renderer::DrawTracerResult(GLuint textureId) {
        this->m_programTex->Use();

         glm::mat4x4 identity = glm::mat4x4(1.0f);

        this->m_programTex->SetUniform("view", identity);
        this->m_programTex->SetUniform("projection", identity);
        this->m_program->SetUniform("model", identity);

        GLuint vao, vbo, ebo;

        // VAO
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // VBO
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, m_quad.GetVertexSize() * 4, m_quad.GetVertices(), GL_STATIC_DRAW);

        // EBO - render order
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, m_quad.GetIndicies(), GL_STATIC_DRAW);

        // setting data layout
        this->m_programTex->SetAttribute("position", 3, GL_FLOAT, GL_FALSE, m_quad.GetVertexSize(), 0);
        this->m_programTex->SetAttribute("textureCoords", 2, GL_FLOAT, GL_FALSE, m_quad.GetVertexSize(), (void*)(3 * sizeof(GLfloat)));

        // traced texture (texture unit 1)
        glBindTexture(GL_TEXTURE_2D, textureId);
        glActiveTexture(GL_TEXTURE1);
        glUniform1i(this->m_programTex->GetUniform("tex"), 1);

        // draw
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // cleanup
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);
    }

    void Renderer::SetPolygonMode(GLenum mode) {
        glPolygonMode(GL_FRONT_AND_BACK, mode);

        this->m_polygonMode = mode;
    }

    GLenum Renderer::GetPolygonMode() {
        return this->m_polygonMode;
    }
}
