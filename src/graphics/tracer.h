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
        GLSLProgram* m_program;
        scene::Object* m_rays;

        GLuint m_resultTexture;

        //TODO extract
        void BuildRays(glm::vec3 origin, glm::vec4* hits, unsigned int num_rays);
        glm::vec3 CalculateCornerRay(const glm::vec4& corner, scene::Camera& camera);

    public:
        Tracer();
        ~Tracer() override;

        void Draw(scene::Scene& scene, scene::Camera& cam, unsigned int width, unsigned int height);

        bool HasResult();
        scene::Object& GetRays();

        GLuint GetResultTexture();
    };
}
#endif // TRACER_H
