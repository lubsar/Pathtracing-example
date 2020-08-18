#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <QOpenGLFunctions_4_5_Core>

#include "renderer.h"
#include "glslprogram.h"

namespace graphics {
    class Rasterizer : public Renderer, protected QOpenGLFunctions_4_5_Core
    {
    private:
        GLSLProgram* program;
        GLint model_location, projection_location, view_location;

    public:
        Rasterizer();
        virtual ~Rasterizer() override;

        virtual void Init() override;
        virtual void Clear() override;

        virtual void Draw(scene::Scene& scene, scene::Camera& camera) override;
        virtual void Draw(scene::Object& obj, scene::Camera& camera) override;
    };
}
#endif // RASTERIZER_H
