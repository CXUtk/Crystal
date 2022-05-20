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
#include <Function/Framework/Components/Light/Lights/SphereEnvironmentLight.h>
#include <Function/Framework/Components/Light/Lights/CubemapEnvironmentLight.h>

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
        auto scene = Scene::LoadScene(File::ReadAllText("assets/engine/scene/Knob.json"));

        //auto pureWhite = std::make_shared<CPUTexture2DPure>(Spectrum(1.f));
        //auto pureRed = std::make_shared<CPUTexture2DPure>(Spectrum(1.f, 0.f, 0.f));
        //auto pureCyan = std::make_shared<CPUTexture2DPure>(Spectrum(0.f, 0.5f, 1.f));
        //auto image = std::make_shared<CPUTexture2DCheckerBoard>(Spectrum(1.f), Spectrum(0.33f));//assetManager->LoadAsset<CPUTexture2D>("engine:LUT_cpu");
        //auto checkerBoard = std::make_shared<CPUTexture2DCheckerBoard>(Spectrum(.3f), Spectrum(0.1f, 0.1f, 0.f));
        //auto pureSmooth = std::make_shared<CPUTexture2DPure>(Spectrum(0.1f));

        RenderProperties renderprops = {};
        renderprops.FrameBufferSize = window->GetWindowSize();
        renderprops.Gamma = 2.2;
        renderprops.SampleCount = 64;
        renderprops.NumOfThreads = 8;

        Transform transform;
        //renderprops.EnvironmentLight = std::make_shared<SphereEnvironmentLight>(transform,
        //    std::make_shared<CPUTexture2DPure>(Spectrum(std::pow(0.5f, 2.2f))));

        auto t = assetManager->LoadAsset<CPUTexture2D>("engine:Environment/DiningRoom");
        renderprops.EnvironmentLight = std::make_shared<SphereEnvironmentLight>(transform,
            t);


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