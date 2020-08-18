#include "renderer.h"

using namespace scene;

namespace graphics {
    Renderer::Renderer(){}
    Renderer::~Renderer(){}

    void Renderer::Init(){}
    void Renderer::Clear(){}

    void Renderer::Draw(Scene& scene, Camera& camera){}
    void Renderer::Draw(Object& obj, Camera& camera){}
}
