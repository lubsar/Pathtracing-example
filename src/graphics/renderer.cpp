#include "renderer.h"

#include "util/shader.h"
#include "gtc/type_ptr.hpp"

using namespace scene;

namespace graphics {
    Renderer::Renderer()  {
        initializeOpenGLFunctions();

        using namespace std;

        // setup shaders
        string vertex_src = util::load_shader_source("shaders/renderer.vert");
        string fragment_src = util::load_shader_source("shaders/renderer.frag");

        GLuint vertex_shader, fragment_shader;

        const GLchar* source = vertex_src.c_str();
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &source, NULL);
        glCompileShader(vertex_shader);

        source = fragment_src.c_str();
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &source, NULL);
        glCompileShader(fragment_shader);

        this->m_program = glCreateProgram();
        glAttachShader(this->m_program, vertex_shader);
        glAttachShader(this->m_program, fragment_shader);
        glBindFragDataLocation(this->m_program, 0, "outColor");
        glLinkProgram(this->m_program);
        glUseProgram(this->m_program);

        //uniform locations
        this->m_modelLocation = glGetUniformLocation(this->m_program, "model");
        this->m_viewLocation = glGetUniformLocation(this->m_program,"view");
        this->m_projectionLocation = glGetUniformLocation(this->m_program,"projection");

        //cleanup
        glDetachShader(this->m_program, vertex_shader);
        glDetachShader(this->m_program, fragment_shader);
        glUseProgram(0);
    }

    Renderer::~Renderer() {}

    void Renderer::Prepare() {
        glUseProgram(this->m_program);

        glEnable(GL_DEPTH_TEST);
    }

    void Renderer::Clear() {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::Draw(scene::Scene& scene, scene::Camera& camera) {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // camera matrices
        glUniformMatrix4fv(this->m_viewLocation, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
        glUniformMatrix4fv(this->m_projectionLocation, 1, GL_FALSE, glm::value_ptr(camera.CalculateProjectionMatrix()));

        for(Object* obj : *(scene.GetObjects())) {
          this->Draw(*obj);
        }
    }

    void Renderer::Draw(scene::Object& obj, scene::Camera& camera) {
        // camera matrices
        glUniformMatrix4fv(this->m_viewLocation, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
        glUniformMatrix4fv(this->m_projectionLocation, 1, GL_FALSE, glm::value_ptr(camera.CalculateProjectionMatrix()));

        Draw(obj);
    }

    void Renderer::Draw(scene::Object& obj) {
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
        position_location = glGetAttribLocation(this->m_program, "position");
        glEnableVertexAttribArray(position_location);
        glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

        color_location = glGetAttribLocation(this->m_program,"color");
        glEnableVertexAttribArray(color_location);
        glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(GLfloat)));

        // model matrix
        glUniformMatrix4fv(this->m_modelLocation, 1, GL_FALSE, glm::value_ptr(obj.GetTransformation()));

        // draw
        glDrawElements(mesh.ElementType(), mesh.LenIndices(), GL_UNSIGNED_INT, 0);

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
