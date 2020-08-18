#ifndef SCENE_H
#define SCENE_H

#include "object.h"

#include <vector>

namespace scene {
    class Scene
    {
    private:
        bool isCurrent;
        std::vector<Object*>* objects;

    public:
        Scene();

        std::vector<Object*>* GetObjects();
        void AddObject(Object* object);
    };
}

#endif // SCENE_H
