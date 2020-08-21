#include "tracer.h"

#include "util/shader.h"
#include "gtc/type_ptr.hpp"

#include <iostream>

using namespace scene;

namespace graphics {
    Tracer::Tracer()
    {
        initializeOpenGLFunctions();

        this->m_rays = new Object();
        this->m_rays->GetMesh().SetElementType(GL_LINES);

        std::string compute_src = util::load_shader_source("shaders/tracer.glsl");

        const GLchar* source = compute_src.c_str();
        GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        this->m_program = glCreateProgram();
        glAttachShader(this->m_program, shader);
        glLinkProgram(this->m_program);
        glUseProgram(this->m_program);

        //cleanup
        glDetachShader(this->m_program, shader);
    }

    Tracer::~Tracer() {
        delete this->m_rays;
    }

    void Tracer::Prepare(){
        glUseProgram(this->m_program);
    }

    void Tracer::Draw(scene::Scene& scene, scene::Camera& camera) {
        int imageWidth = 100;
        int imageHeight = 100;

        // camera
        glm::vec3 eye, top_left, top_right, bottom_left, bottom_right;

        eye = {0.0f, 0.0f, 5.0f};
        top_left = {-1.0f, 1.0f, 0.0f};
        top_right = {1.0f, 1.0f, 0.0f};
        bottom_left = {-1.0f, -1.0f, 0.0f};
        bottom_right = {1.0f, -1.0f, 0.0f};

        //TODO objects

        // spheres
        unsigned int num_spheres = scene.GetSpheres()->size();
        glm::vec4* spheres = new glm::vec4[num_spheres];

        int i = 0;
        for(Sphere* s : *scene.GetSpheres()) {
            spheres[i++] = glm::vec4(s->m_center, s->m_radius);
        }

        // shader
        GLuint spheres_ssbo, debug_ssbo, output_image;
        glGenBuffers(1, &spheres_ssbo);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, spheres_ssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * num_spheres, spheres, GL_STATIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, spheres_ssbo);

        glGenBuffers(1, &debug_ssbo);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, debug_ssbo);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, debug_ssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * imageWidth * imageHeight, nullptr, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        glGenTextures(1, &output_image);
        glBindTexture(GL_TEXTURE_2D, output_image);
        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA32F, imageWidth, imageHeight, 0,GL_RGBA, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glGenerateMipmap(GL_TEXTURE_2D);

        // uniforms
        glBindImageTexture(1, output_image, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

        glUniform3fv(glGetUniformLocation(this->m_program, "eye"), 1, glm::value_ptr(eye));
        glUniform3fv(glGetUniformLocation(this->m_program, "top_left"), 1, glm::value_ptr(top_left));
        glUniform3fv(glGetUniformLocation(this->m_program, "top_right"), 1, glm::value_ptr(top_right));
        glUniform3fv(glGetUniformLocation(this->m_program, "bottom_left"), 1, glm::value_ptr(bottom_left));
        glUniform3fv(glGetUniformLocation(this->m_program, "bottom_right"), 1, glm::value_ptr(bottom_right));

        // compute
        GLint group_size[3];
        glGetProgramiv(this->m_program, GL_COMPUTE_WORK_GROUP_SIZE, &group_size[0]);

        int work_x = imageWidth / group_size[0] + (imageWidth % group_size[0] ? 1 : 0);
        int work_y = imageHeight / group_size[1] + (imageHeight % group_size[1] ? 1 : 0);

        glDispatchCompute(work_x, work_y, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);

        // retrieve ray data
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, debug_ssbo);

        glm::vec4 *ptr;
        ptr = ( glm::vec4 *) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
        //build traced rays geometry
        BuildRays(eye, ptr, imageWidth * imageHeight);

        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        delete[] spheres;
    }

    bool Tracer::HasRays() {
        return this->m_rays->GetMesh().LenVertices() > 0;
    }

    scene::Object& Tracer::GetRays() {
       return *this->m_rays;
    }

    void Tracer::BuildRays(glm::vec3 origin, glm::vec4* hits, unsigned int num_rays) {
        Mesh& mesh = this->m_rays->GetMesh();
        mesh.Clear();
        mesh.AddVertex({origin.x, origin.y, origin.z, 0.0f, 0.0f, 0.0f});

        for(unsigned int i = 0; i < num_rays; i++) {
            glm::vec4 point = hits[i];

            //origin has index 0
            mesh.AddIndices({0, mesh.AddVertex({point.x, point.y, point.z, 0.0f, 0.0f, 0.0f})});
        }
    }
}
