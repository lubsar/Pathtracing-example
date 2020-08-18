#ifndef TRACER_H
#define TRACER_H

#include "renderer.h"

namespace graphics {
    class Tracer : public Renderer
    {
    public:
        Tracer();
        ~Tracer() override;

        virtual void Init() override;
        virtual void Clear() override;

        virtual void Draw(scene::Scene& scene, scene::Camera& camera) override;
        virtual void Draw(scene::Object& obj, scene::Camera& camera) override;
    };
}
#endif // TRACER_H
