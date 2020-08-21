#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <QOpenGLFunctions_4_5_Core>

#include "scene/scene.h"
#include "scene/camera.h"

namespace graphics {
    class Renderer : protected QOpenGLFunctions_4_5_Core
    {
    private:
        GLuint m_program;
        GLint m_modelLocation, m_projectionLocation, m_viewLocation;
        GLenum m_polygonMode;

        void Draw(scene::Object&);

    public:
        Renderer();
        ~Renderer();

        void Prepare();
        void Clear();

        void Draw(scene::Scene& scene, scene::Camera& camera);
        void Draw(scene::Object& obj, scene::Camera& camera);

        void SetPolygonMode(GLenum mode);
        GLenum GetPolygonMode();
    };
}
#endif // RASTERIZER_H
