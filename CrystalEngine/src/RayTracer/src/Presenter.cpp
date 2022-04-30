#include "Presenter.h"

#include <Core/Utils/Logger.h>
#include <Core/Math/Geometry.h>
#include <Core/Utils/GameTimer.h>

#include <Function/Input/InputController.h>
#include <Function/Render/SpriteBatch.h>
#include <Function/UI/UIStateMachine.h>

#include <Resource/Asset/AssetManager.h>

#include <Function/Framework/Object/GameObject.h>
#include <Function/Framework/Components/Transform/TransformComponent.h>
#include <Function/Framework/Components/Mesh/MeshComponent.h>
#include <Function/Framework/Components/Shape/ShapeComponent.h>
#include <Function/Framework/Components/Shape/Shapes/Sphere.h>
#include <Function/Framework/Components/Light/LightComponent.h>
#include <Function/Framework/Components/Light/PointLight.h>
#include <Function/Framework/Components/Material/Materials/LambertianMaterial.h>

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
        auto graphicsDevice = m_engine->GetGraphicsDevice();
        auto window = m_engine->GetWindow();
        
        Texture2DDescription texDesc = {};
        texDesc.Format = RenderFormat::RGBA8ub;
        texDesc.MipmapLevels = 1;
        texDesc.Size = window->GetWindowSize();
        texDesc.Usage = BufferUsage::Default;

        m_rayTraceTexture = graphicsDevice->CreateTexture2DFromMemory(nullptr, 0, texDesc);

        auto assetManager = m_engine->GetAssetManager();
        auto scene = std::make_shared<Scene>();

        RenderProperties renderprops = {};
        renderprops.FrameBufferSize = window->GetWindowSize();
        renderprops.Gamma = 2.2;
        renderprops.SampleCount = 1;

        Transform transform(Vector3f(0.f, -1.f, 0.f), Vector3f(5.f, 1.f, 5.f), glm::identity<Quaternion>());

        auto plane = std::make_shared<GameObject>();
        plane->AddComponent(std::make_shared<TransformComponent>(transform));
        plane->AddComponent(std::make_shared<MeshComponent>(assetManager->LoadAsset<Mesh>("engine:Floor")));
        plane->AddComponent(std::make_shared<MaterialComponent>(std::make_shared<LambertianMaterial>(Spectrum(1.f))));


        Transform transform_sphere;
        auto sphere = std::make_shared<GameObject>();
        auto sphereShape = std::make_shared<Sphere>(1, transform_sphere);
        sphere->AddComponent(std::make_shared<TransformComponent>(transform));
        sphere->AddComponent(std::make_shared<ShapeComponent>(sphereShape));
        sphere->AddComponent(std::make_shared<MaterialComponent>(std::make_shared<LambertianMaterial>(Spectrum(1.f))));


        auto targetPos = Vector3f(0.f);
        auto viewPos = Vector3f(0, 2, 5);
        Transform transform_camera(viewPos, Vector3f(1.f),
            glm::quatLookAt(glm::normalize(targetPos - viewPos), Vector3f(0, 1, 0)));

        auto camera = std::make_shared<GameObject>();
        auto normalCamera = std::make_shared<Camera>(glm::pi<float>() / 3.f,
            (float)renderprops.FrameBufferSize.x / renderprops.FrameBufferSize.y, 0, 100);
        camera->AddComponent(std::make_shared<TransformComponent>(transform_camera));
        camera->AddComponent(std::make_shared<CameraComponent>(normalCamera));

        Transform transform_light(Vector3f(0, 5, 0), Vector3f(1.f),
            glm::identity<Quaternion>());
        auto pointLight = std::make_shared<PointLight>(Spectrum(10.f));
        auto light = std::make_shared<GameObject>();
        light->AddComponent(std::make_shared<TransformComponent>(transform_light));
        light->AddComponent(std::make_shared<LightComponent>(pointLight));

        scene->AddObject(plane);
        scene->AddObject(sphere);
        scene->AddObject(camera);
        scene->AddObject(light);

        m_tracer = std::make_shared<RayTracer>();
        m_tracer->RenderAsync(scene, renderprops, cptr(camera->GetComponent<CameraComponent>()));
    }

    void Presenter::Update(const crystal::GameTimer& gameTimer)
    {

        if (m_elapsedTime > 0.5)
        {
            auto data = m_tracer->GetTexture2D();
            size_t sz = sizeof(int) * m_rayTraceTexture->GetSize().x * m_rayTraceTexture->GetSize().y;
            m_rayTraceTexture->ReplaceContent(data.get(), sz, 0);
            m_elapsedTime = 0.0;
        }
        m_elapsedTime += gameTimer.GetPhysicalDeltaTime();
    }

    void Presenter::Draw(const crystal::GameTimer& gameTimer)
    {
        auto graphicsContext = m_engine->GetGraphicsContext();
        auto spriteBatch = m_engine->GetSpriteBatch();
        graphicsContext->Clear(
            crystal::ClearOptions::CRYSTAL_CLEAR_TARGET
            | crystal::ClearOptions::CRYSTAL_CLEAR_DEPTH
            | crystal::ClearOptions::CRYSTAL_CLEAR_STENCIL,
            crystal::Color4f(0.f, 0.f, 0.f, 0.f), 1.0f, 0.f);

        auto window = m_engine->GetWindow();
        auto bound = Bound2i(Vector2i(0), window->GetWindowSize());
        spriteBatch->Begin();
        spriteBatch->Draw(m_rayTraceTexture, bound, Color4f(1.f));
        spriteBatch->End();
    }

    void Presenter::Exit()
    {
        m_tracer->Save();
    }

    bool Presenter::Paused()
    {
        return false;
    }
}