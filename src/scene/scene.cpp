#include "scene.h"

namespace scene {
    Scene::Scene()
    {
        this->m_objects = new std::vector<Object*>();
        this->m_spheres = new std::vector<Sphere*>();
    }

    Scene::~Scene() {
        delete m_objects;
        delete m_spheres;
    }

    void Scene::AddObject(Object* obj) {
        this->m_objects->push_back(obj);
    }

    std::vector<Object*>* Scene::GetObjects() {
        return this->m_objects;
    }

    void Scene::AddSphere(Sphere* sphere) {
        this->m_spheres->push_back(sphere);
    }

    std::vector<Sphere*>* Scene::GetSpheres() {
        return this->m_spheres;
    }
}
