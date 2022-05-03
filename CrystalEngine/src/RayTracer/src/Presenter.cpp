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
#include <Function/Framework/Components/Light/Lights/PointLight.h>
#include <Function/Framework/Components/Light/Lights/DiffusedAreaLight.h>

#include <Function/Framework/Components/Material/Materials/LambertianMaterial.h>
#include <Function/Framework/Components/Material/Materials/MirrorMaterial.h>
#include <Function/Framework/Components/Material/Materials/GlassMaterial.h>
#include <Function/Framework/Components/Material/Materials/MicrofacetMaterial.h>
#include <Function/Framework/Components/Material/Materials/PlasticMaterial.h>

#include <Core/Sampling/Sampling.h>
#include <Platform/RHI/FileSystem/File.h>

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
        auto scene = Scene::LoadScene(File::ReadAllText("assets/engine/scene/sibenik.json"));

        auto pureWhite = std::make_shared<CPUTexture2DPure>(Spectrum(1.f));
        auto pureRed = std::make_shared<CPUTexture2DPure>(Spectrum(1.f, 0.f, 0.f));
        auto pureCyan = std::make_shared<CPUTexture2DPure>(Spectrum(0.f, 0.5f, 1.f));
        auto image = std::make_shared<CPUTexture2DCheckerBoard>(Spectrum(1.f), Spectrum(0.33f));//assetManager->LoadAsset<CPUTexture2D>("engine:LUT_cpu");
        auto checkerBoard = std::make_shared<CPUTexture2DCheckerBoard>(Spectrum(.3f), Spectrum(0.1f, 0.1f, 0.f));
        auto pureSmooth = std::make_shared<CPUTexture2DPure>(Spectrum(0.1f));

        RenderProperties renderprops = {};
        renderprops.FrameBufferSize = window->GetWindowSize();
        renderprops.Gamma = 2.2;
        renderprops.SampleCount = 1;
        renderprops.NumOfThreads = 8;
        renderprops.Skybox = assetManager->LoadAsset<CPUTextureCubemap>("engine:Cubemaps/Sky/Skybox_cpu");


        //SJson::JsonNode areaLightJsonNode;
        //DiffuseAreaLightSettings areaLightSetting = {};
        //areaLightSetting.LEmit = Spectrum(5.f);
        //areaLightSetting.NumSamples = 1;
        //areaLightJsonNode["Type"] = "DiffuseAreaLight";
        //areaLightJsonNode["Data"] = SJson::serialize(areaLightSetting);

        //Transform transform(Vector3f(0.f, -1.f, 0.f), Vector3f(5.f, 1.f, 5.f), glm::identity<Quaternion>());

        //auto plane = std::make_shared<GameObject>();
        //plane->AddComponent(std::make_shared<TransformComponent>(transform));
        //plane->AddComponent(std::make_shared<MeshComponent>(assetManager->LoadAsset<Mesh>("engine:Floor")));
        //plane->AddComponent(std::make_shared<MaterialComponent>(std::make_shared<LambertianMaterial>(image)));



        //Transform transform_sphere;
        //auto sphere = std::make_shared<GameObject>();
        //SJson::JsonNode sphereJsonNode;
        //SphereSettings spSetting = {};
        //spSetting.Radius = 1.f;
        //sphereJsonNode["Type"] = "Sphere";
        //sphereJsonNode["Data"] = SJson::serialize(spSetting);

        //sphere->AddComponent(std::make_shared<TransformComponent>(transform_sphere));
        //sphere->AddComponent(std::make_shared<ShapeComponent>(sphereJsonNode));
        //sphere->AddComponent(std::make_shared<MaterialComponent>(std::make_shared<GlassMaterial>(pureWhite, pureWhite, 1.5f)));

        //Transform transform_sphere1;
        //transform_sphere1.SetTranslation(Vector3f(2, 0, 1.f));
        //auto sphere1 = std::make_shared<GameObject>();
        //sphere1->AddComponent(std::make_shared<TransformComponent>(transform_sphere1));
        //sphere1->AddComponent(std::make_shared<ShapeComponent>(sphereJsonNode));
        //sphere1->AddComponent(std::make_shared<MaterialComponent>(std::make_shared<PlasticMaterial>(pureRed, checkerBoard, 1.5)));

        //Transform transform_sphere2;
        //transform_sphere2.SetTranslation(Vector3f(-2.5, -1, 0));
        //transform_sphere2.SetScale(Vector3f(0.4, 0.4, 0.4));
        ////Quaternion rot = glm::identity<Quaternion>();
        ////transform_sphere2.SetRotation(glm::rotate(rot, glm::pi))

        //auto sphere2 = std::make_shared<GameObject>();
        //spSetting.Radius = .5f;
        //sphereJsonNode["Data"] = SJson::serialize(spSetting);


        //sphere2->AddComponent(std::make_shared<TransformComponent>(transform_sphere2));
        //sphere2->AddComponent(std::make_shared<MeshComponent>(assetManager->LoadAsset<Mesh>("engine:sibenik")));
        ////sphere2->AddComponent(std::make_shared<ShapeComponent>(sphereJsonNode));
        //sphere2->AddComponent(std::make_shared<MaterialComponent>(std::make_shared<LambertianMaterial>(pureWhite)));
        ////sphere2->AddComponent(std::make_shared<MaterialComponent>(std::make_shared<PlasticMaterial>(pureCyan, pureSmooth, 1.5)));

        ////auto areaLight = std::make_shared<DiffusedAreaLight>()
        ////sphere2->AddComponent(std::make_shared<LightComponent>(std::make_shared<LightComponent>()));


        ////std::mt19937 mt;
        ////for (int i = 0; i < 50; i++)
        ////{
        ////    Transform transform_spherex;
        ////    transform_spherex.SetTranslation(Vector3f(UniformRandomFloat(mt) * 6 - 3, UniformRandomFloat(mt), UniformRandomFloat(mt) * 6 - 3));
        ////    auto spherex = std::make_shared<GameObject>();
        ////    auto sphereShapex = std::make_shared<Sphere>(UniformRandomFloat(mt) * 0.3f + 0.2f, transform_spherex);
        ////    spherex->AddComponent(std::make_shared<TransformComponent>(transform_spherex));
        ////    spherex->AddComponent(std::make_shared<ShapeComponent>(sphereShapex));
        ////    spherex->AddComponent(std::make_shared<MaterialComponent>(std::make_shared<LambertianMaterial>(Spectrum(UniformRandomFloat(mt), UniformRandomFloat(mt), UniformRandomFloat(mt)))));
        ////    scene->AddObject(spherex);
        ////}

        ////for (int i = 0; i < 5; i++)
        ////{
        ////    Transform transform_light(Vector3f(UniformRandomFloat(mt) * 10 - 5, UniformRandomFloat(mt) * 2.f, UniformRandomFloat(mt) * 10 - 5), Vector3f(1.f),
        ////        glm::identity<Quaternion>());
        ////    auto pointLight = std::make_shared<PointLight>(Spectrum(UniformRandomFloat(mt) * 4, UniformRandomFloat(mt) * 4, UniformRandomFloat(mt) * 4));
        ////    auto light = std::make_shared<GameObject>();
        ////    light->AddComponent(std::make_shared<TransformComponent>(transform_light));
        ////    light->AddComponent(std::make_shared<LightComponent>(pointLight));
        ////    scene->AddObject(light);
        ////}

        //auto camera = std::make_shared<GameObject>();

        //auto targetPos = Vector3f(0.f, -5.f, 0.f);
        //auto viewPos = Vector3f(5.f, -5, 0);
        //Transform transform_camera(viewPos, Vector3f(1.f),
        //    glm::quatLookAt(glm::normalize(targetPos - viewPos), Vector3f(0, 1, 0)));
        //SJson::JsonNode cameraJsonNode;
        //PerspectiveCameraSetting perspSetting = {};
        //perspSetting.FovY = glm::pi<float>() / 3.f;
        //perspSetting.Aspect = (float)renderprops.FrameBufferSize.x / renderprops.FrameBufferSize.y;
        //perspSetting.ZNear = 0;
        //perspSetting.ZFar = 100;
        //cameraJsonNode["Type"] = "Perspective";
        //cameraJsonNode["Data"] = SJson::serialize(perspSetting);

        //camera->AddComponent(std::make_shared<TransformComponent>(transform_camera));
        //camera->AddComponent(std::make_shared<CameraComponent>(cameraJsonNode));


        //auto light = std::make_shared<GameObject>();

        //Quaternion flip = glm::identity<Quaternion>();
        //flip = glm::rotate(flip, glm::pi<float>(), Vector3f(1, 0, 0));

        //Transform transform_light(Vector3f(-5, -1, 0), Vector3f(1.f),
        //    flip);

        //SJson::JsonNode lightJsonNode;
        //PointLightSettings ptlSetting = {};
        //ptlSetting.Intensity = Spectrum(20.f);
        //lightJsonNode["Type"] = "DiffuseAreaLight";
        //lightJsonNode["Data"] = SJson::serialize(areaLightSetting);
        //light->AddComponent(std::make_shared<MeshComponent>(assetManager->LoadAsset<Mesh>("engine:Floor")));
        //light->AddComponent(std::make_shared<TransformComponent>(transform_light));
        //light->AddComponent(std::make_shared<LightComponent>(lightJsonNode));
        //light->AddComponent(std::make_shared<MaterialComponent>(std::make_shared<LambertianMaterial>(pureWhite)));

        ////scene->AddObject(plane);
        ////scene->AddObject(sphere);
        ////scene->AddObject(sphere1);
        //scene->AddObject(sphere2);
        //scene->AddObject(camera);
        //scene->AddObject(light);

        scene->Initialize();

        m_tracer = std::make_shared<RayTracer>();

        m_tracer->RenderAsync(scene, renderprops, scene->GetCameraComponents()[0]);
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