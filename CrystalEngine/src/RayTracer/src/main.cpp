#include <Engine.h>
#include "Presenter.h"
#include "Function/Framework/Framework.h"
#include "Function/Framework/Components/Mesh/MeshComponent.h"
#include "Function/Framework/Components/Transform/TransformComponent.h"
#include "Function/Framework/Object/GameObject.h"

using namespace crystal;
int main(int argc, char** argv)
{
    glm::vec3 A(0, 0, 0);
    glm::vec3 B(1, 0, -1);
    glm::vec3 C(0, -2, -1);
    glm::vec3 D(0, 2, -1);
    glm::vec3 E(-1, 0, -1);

    auto eyePos = glm::vec3(0, 1, -5);
    auto objectPos = glm::vec3(0, 0, 0);
    auto n1 = glm::quatLookAt(glm::normalize(objectPos - eyePos), glm::vec3(0, 1, 0));

    Engine* engine = Engine::GetInstance();
    auto uptr = std::make_unique<tracer::Presenter>();
    engine->Start(std::move(uptr));
    //GameObject gameObject;
    //gameObject.AddComponent(std::make_shared<TransformComponent>(&gameObject));
    //gameObject.AddComponent(std::make_shared<MeshComponent>(&gameObject));

    //auto mesh = gameObject.GetComponent<MeshComponent>();
    //auto transform = gameObject.GetComponent<TransformComponent>();
    return 0;
}
