#include "scene.h"

namespace scene {
    Scene::Scene()
    {
        this->isCurrent = false;
        this->objects = new std::vector<Object*>();
    }

    void Scene::AddObject(Object* obj) {
        this->objects->push_back(obj);
        this->isCurrent = false;
    }

    std::vector<Object*>* Scene::GetObjects() {
        return this->objects;
    }
}
