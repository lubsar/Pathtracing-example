#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <QOpenGLFunctions_4_5_Core>

#include "glslprogram.h"
#include "texturedquad.h"

#include "scene/scene.h"
#include "scene/camera.h"
#include "scene/meshsphere.h"

namespace graphics {
    class Renderer : protected QOpenGLFunctions_4_5_Core
    {
    private:
        GLSLProgram* m_program;
        GLSLProgram* m_programInst;
        GLSLProgram* m_programTex;

        GLenum m_polygonMode;

        scene::MeshSphere m_sphere;
        TexturedQuad m_quad;

        void Draw(scene::Object&);
        void Draw(std::vector<scene::Sphere*>* spheres);

    public:
        Renderer();
        ~Renderer();

        void Prepare();
        void Clear();

        void Draw(scene::Scene& scene, scene::Camera& camera);
        void Draw(scene::Object& obj, scene::Camera& camera);
        void DrawTracerResult(GLuint textureId);

        void SetPolygonMode(GLenum mode);
        GLenum GetPolygonMode();
    };
}
#endif // RASTERIZER_H
