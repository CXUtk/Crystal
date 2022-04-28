#include <Engine.h>
#include "Presenter.h"
#include "Function/Framework/Framework.h"
#include "Function/Framework/Components/Mesh/MeshComponent.h"
#include "Function/Framework/Components/Transform/TransformComponent.h"
#include "Function/Framework/Object/GameObject.h"

using namespace crystal;
int main(int argc, char** argv)
{
    //Engine* engine = Engine::GetInstance();
    //auto uptr = std::make_unique<tracer::CrystalTracer>();
    //engine->Start(std::move(uptr));
    GameObject gameObject;
    gameObject.AddComponent(std::make_shared<TransformComponent>(&gameObject));
    gameObject.AddComponent(std::make_shared<MeshComponent>(&gameObject));

    auto mesh = gameObject.GetComponent<MeshComponent>();
    auto transform = gameObject.GetComponent<TransformComponent>();
    return 0;
}
