#include "Presenter.h"

#include <Core/Utils/Logger.h>
#include <Core/Math/Geometry.h>
#include <Core/Utils/GameTimer.h>

#include <Function/Input/InputController.h>
#include <Function/Render/SpriteBatch.h>
#include <Function/UI/UIStateMachine.h>

#include <Function/Framework/Object/GameObject.h>
#include <Function/Framework/Components/Transform/TransformComponent.h>
#include <Function/Framework/Components/Mesh/MeshComponent.h>
#include <Function/Framework/Components/Shape/ShapeComponent.h>

using namespace crystal;

namespace tracer
{
    Presenter::Presenter()
        : crystal::Application()
    {}

    Presenter::~Presenter()
    {}

    void Presenter::Initialize()
    {
        auto assetManager = m_engine->GetAssetManager();
        auto scene = std::make_shared<Scene>();

        Transform transform;

        auto plane = std::make_shared<GameObject>();
        plane->AddComponent(std::make_shared<MeshComponent>());
        plane->AddComponent(std::make_shared<TransformComponent>(transform));

        auto sphere = std::make_shared<GameObject>();
        sphere->AddComponent(std::make_shared<ShapeComponent>());
        sphere->AddComponent(std::make_shared<TransformComponent>(transform));

        scene->AddObject(plane);
        scene->AddObject(sphere);

        m_tracer = std::make_shared<RayTracer>();

        RenderProperties renderprops = {};
        renderprops.FrameBufferSize = Vector2i(800, 600);
        renderprops.Gamma = 2.2;
        renderprops.SampleCount = 1;

        m_tracer->RenderAsync(scene, renderprops);
    }

    void Presenter::Update(const crystal::GameTimer& gameTimer)
    {
    }

    void Presenter::Draw(const crystal::GameTimer& gameTimer)
    {
        auto graphicsContext = m_engine->GetGraphicsContext();
        graphicsContext->Clear(
            crystal::ClearOptions::CRYSTAL_CLEAR_TARGET
            | crystal::ClearOptions::CRYSTAL_CLEAR_DEPTH
            | crystal::ClearOptions::CRYSTAL_CLEAR_STENCIL,
            crystal::Color4f(0.f, 0.f, 0.f, 0.f), 1.0f, 0.f);
    }

    void Presenter::Exit()
    {

    }

    bool Presenter::Paused()
    {
        return false;
    }
}