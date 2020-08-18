#ifndef RENDERER_H
#define RENDERER_H

#include "scene/camera.h"
#include "scene/scene.h"
#include "scene/object.h"

namespace graphics {
    class Renderer
    {
    public:
        Renderer();
        virtual ~Renderer();

        virtual void Init();
        virtual void Clear();

        virtual void Draw(scene::Scene& scene, scene::Camera& camera);
        virtual void Draw(scene::Object& obj, scene::Camera& camera);
    };
}

#endif // RENDERER_H
