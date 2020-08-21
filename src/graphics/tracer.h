#ifndef TRACER_H
#define TRACER_H

#include "renderer.h"
#include "scene/object.h"
#include "scene/scene.h"
#include "scene/camera.h"

#include <QOpenGLFunctions_4_5_Core>
#include <vector>

namespace graphics {
    class Tracer : protected QOpenGLFunctions_4_5_Core
    {
    private:
        GLuint m_program;
        scene::Object* m_rays;

        void BuildRays(glm::vec3 origin, glm::vec4* hits, unsigned int num_rays);

    public:
        Tracer();
        ~Tracer() override;

        void Prepare();
        void Clear();

        void Draw(scene::Scene& scene, scene::Camera& camera);

        bool HasRays();
        scene::Object& GetRays();
    };
}
#endif // TRACER_H
