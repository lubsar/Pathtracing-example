#ifndef SCENE_H
#define SCENE_H

#include "object.h"
#include "sphere.h"

#include <vector>

namespace scene {
    class Scene
    {
    private:
        std::vector<Object*>* m_objects;
        std::vector<Sphere*>* m_spheres;

    public:
        Scene();
        ~Scene();

        std::vector<Object*>* GetObjects();
        void AddObject(Object* object);

        std::vector<Sphere*>* GetSpheres();
        void AddSphere(Sphere* sphere);
    };
}

#endif // SCENE_H
