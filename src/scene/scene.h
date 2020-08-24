#ifndef SCENE_H
#define SCENE_H

#include "object.h"
#include "sphere.h"
#include "sphericallight.h"

#include <vector>

namespace scene {
    class Scene
    {
    private:
        std::vector<Object*>* m_objects;
        std::vector<Sphere*>* m_spheres;
        SphericalLight* m_light;

    public:
        Scene();
        ~Scene();

        std::vector<Object*>* GetObjects();
        void AddObject(Object* object);

        std::vector<Sphere*>* GetSpheres();
        void AddSphere(Sphere* sphere);

        void SetLight(SphericalLight* light);
        SphericalLight* GetLight();
    };
}

#endif // SCENE_H
