#include "tracer.h"

#include "util/shader.h"
#include "gtc/type_ptr.hpp"

#include <iostream>

using namespace scene;

namespace graphics {
    Tracer::Tracer()
    {
        initializeOpenGLFunctions();

        this->m_program = new GLSLProgram();

        this->m_rays = new Object();
        this->m_rays->GetMesh().SetElementType(GL_LINES);

        //shader
        std::string compute_src = util::load_shader_source("shaders/tracer.glsl");
        this->m_program->AddShader(GL_COMPUTE_SHADER, compute_src);
        this->m_program->Link();

        //uniforms
        this->m_program->AddUniform("eye");
        this->m_program->AddUniform("top_left");
        this->m_program->AddUniform("top_right");
        this->m_program->AddUniform("bottom_left");
        this->m_program->AddUniform("bottom_right");

        this->m_program->AddUniform("light_position");
        this->m_program->AddUniform("light_color");
        this->m_program->AddUniform("light_intensity");
     }

    Tracer::~Tracer() {
        delete this->m_rays;
        delete this->m_program;
    }

    void Tracer::Draw(scene::Scene& scene, scene::Camera& camera, unsigned int imageWidth, unsigned int imageHeight) {
        this->m_program->Use();

        // camera
        glm::vec3 eye, top_left, top_right, bottom_left, bottom_right;

        eye = camera.GetPosition();

        top_left = CalculateCornerRay({-1.0f, 1.0f, -1.0f, 1.0f}, camera);
        top_right = CalculateCornerRay({1.0f, 1.0f, -1.0f, 1.0f}, camera);
        bottom_left = CalculateCornerRay({-1.0f, -1.0f, -1.0f, 1.0f}, camera);
        bottom_right = CalculateCornerRay({1.0f, -1.0f, -1.0f, 1.0f}, camera);

        //TODO objects

        // spheres
        unsigned int num_spheres = scene.GetSpheres()->size();
        glm::vec4* spheres = new glm::vec4[num_spheres * 2];

        int i = 0;
        for(Sphere* s : *scene.GetSpheres()) {
            spheres[i++] = glm::vec4(s->GetCenter(), s->GetRadius());
            spheres[i++] = glm::vec4(s->GetColor(), 1.0f);
        }

        // shader
        GLuint spheres_ssbo, debug_ssbo, output_image;
        glGenBuffers(1, &spheres_ssbo);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, spheres_ssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * 2 * num_spheres, spheres, GL_STATIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, spheres_ssbo);

        glGenBuffers(1, &debug_ssbo);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, debug_ssbo);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, debug_ssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * imageWidth * imageHeight * 2, nullptr, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        glGenTextures(1, &output_image);
        glBindTexture(GL_TEXTURE_2D, output_image);
        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA32F, imageWidth, imageHeight, 0,GL_RGBA, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glGenerateMipmap(GL_TEXTURE_2D);

        // uniforms
        glActiveTexture(GL_TEXTURE1);
        glBindImageTexture(1, output_image, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

        this->m_program->SetUniform("eye", eye);
        this->m_program->SetUniform("top_left", top_left);
        this->m_program->SetUniform("top_right", top_right);
        this->m_program->SetUniform("bottom_left", bottom_left);
        this->m_program->SetUniform("bottom_right", bottom_right);

        // light
        SphericalLight* light = scene.GetLight();

        this->m_program->SetUniform("light_position", light->GetPosition());
        this->m_program->SetUniform("light_color", light->GetColor());
        this->m_program->SetUniform("light_intensity", light->GetIntensity());

        // compute
        GLint group_size[3];
        glGetProgramiv(this->m_program->GetProgramID(), GL_COMPUTE_WORK_GROUP_SIZE, &group_size[0]);

        int groups_x = imageWidth / group_size[0] + (imageWidth % group_size[0] ? 1 : 0);
        int groups_y = imageHeight / group_size[1] + (imageHeight % group_size[1] ? 1 : 0);

        glDispatchCompute(groups_x, groups_y, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);

        // retrieve ray data
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, debug_ssbo);

        glm::vec4 *ptr;
        ptr = ( glm::vec4 *) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
        //build ray geometry
        BuildRays(eye, ptr, imageWidth * imageHeight);

        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        delete[] spheres;
        this->m_resultTexture = output_image;
    }

    bool Tracer::HasResult() {
        return this->m_rays->GetMesh().LenVertices() > 0;
    }

    scene::Object& Tracer::GetRays() {
       return *this->m_rays;
    }

    GLuint Tracer::GetResultTexture() {
        return this->m_resultTexture;
    }

    void Tracer::BuildRays(glm::vec3 origin, glm::vec4* hits, unsigned int num_rays) {
        Mesh& mesh = this->m_rays->GetMesh();
        mesh.Clear();
        mesh.AddVertex({origin.x, origin.y, origin.z, 0.0f, 0.0f, 0.0f});

        for(unsigned int i = 0; i < num_rays; i++) {
            glm::vec4 point = hits[i*2];
            glm::vec4 point2 = hits[i*2 +1];

            //origin has index 0
            unsigned int primary = mesh.AddVertex({point.x, point.y, point.z, 0.0f, 0.0f, 0.0f});
            mesh.AddIndices({0, primary});
            if(point2.w > 0.1f) {
                mesh.AddIndices({primary, mesh.AddVertex({point2.x, point2.y, point2.z, 1.0f, 0.0f, 0.0f})});
            }
        }
    }

    glm::vec3 Tracer::CalculateCornerRay(const glm::vec4 &corner, scene::Camera &camera) {
        glm::mat4 inverseViewProj = glm::inverse(camera.CalculateViewMatrix() * camera.CalculateProjectionMatrix());

        glm::vec4 projected = inverseViewProj * corner;
        glm::vec3 result = glm::vec3(projected);
        result *= 1.0f / projected.w;
        result -= camera.GetPosition();

        return projected;
    }
}
