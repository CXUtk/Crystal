#include "Engine.h"
#include "Application.h"

#include "Core/Utils/GameTimer.h"
#include "Core/Utils/Logger.h"

#include "Function/Input/InputController.h"
#include "Function/Render/RenderExports.h"
#include "Function/UI/UIExports.h"

#include "Resource/Asset/AssetManager.h"

#include "Platform/PlatformFactory.h"
#include "Platform/RHI/FileSystem/File.h"

#include "Core/Pattern/Singleton.h"

namespace crystal
{
    Engine::Engine()
    {
        GlobalLogger::Log(SeverityLevel::Debug, "Engine Construct");
    }
    
    Engine::~Engine()
    {
        m_application.reset();
        m_pUIStateMachine.reset();
        m_pGeometryRenderer.reset();
        m_spriteBatch.reset();
        m_pAssetManager.reset();
        m_inputController.reset();
        m_platformProvider.reset();
        GlobalLogger::Log(SeverityLevel::Debug, "Engine Destruct");
    }

    AssetManager* Engine::GetAssetManager() const
    {
        return ptr(m_pAssetManager);
    }

    IGameWindow* Engine::GetWindow() const
    {
        return m_platformProvider->GetGameWindow();
    }

    InputController* Engine::GetInputController() const
    {
        return ptr(m_inputController);
    }

    IGraphicsDevice* Engine::GetGraphicsDevice() const
    {
        return m_platformProvider->GetGraphicsDevice();
    }

    IGraphicsContext* Engine::GetGraphicsContext() const
    {
        return m_platformProvider->GetGraphicsDevice()->GetContext().get();
    }

    SpriteBatch* Engine::GetSpriteBatch() const
    {
        return ptr(m_spriteBatch);
    }

    GeometryRenderer* Engine::GetGeometryRenderer() const
    {
        return ptr(m_pGeometryRenderer);
    }
    
    //IGraphicsDevice* Engine::GetGraphicsDevice() const
    //{
    //    return m_platformProvider->GetGraphicsDevice();
    //}

    void Engine::m_Initialize()
    {
        ReadConfig();
        GraphicsCommons::InitGraphicsCommons();
        m_platformProvider = PlatformFactory::GetPlatformProvider(m_initArgs);

        m_pAssetManager = std::make_shared<AssetManager>();
        m_pAssetManager->LoadAssetPackage("resources/package1/contents.json");

        auto graphicsDevice = GetGraphicsDevice();
        auto graphicsContext = GetGraphicsContext();

        m_spriteBatch = std::make_unique<SpriteBatch>(graphicsDevice, graphicsContext);
        m_pGeometryRenderer = std::make_unique<GeometryRenderer>(graphicsDevice, graphicsContext);
        m_pUIStateMachine = std::make_unique<UIStateMachine>();

        m_inputController = std::make_unique<InputController>(m_platformProvider->GetGameWindow());
    }

    void Engine::Start(std::unique_ptr<Application>&& application)
    {
        //auto s = &Pilot::PublicSingleton<Test>::getInstance();
        //GlobalLogger::Log(SeverityLevel::Debug, "DLL: %p\n", s);
        m_Initialize();

        auto window = m_platformProvider->GetGameWindow();
        m_application = std::move(application);
        m_application->SetEngine(this);

        m_application->Initialize();

        m_gameTimer.Start();
        double frameBeginTime = 0.0;
        while (!window->ShouldClose())
        {
            frameBeginTime = GameTimer::GetCurrentTime();

            m_gameTimer.Sample();
            if (m_application->Paused() || window->IsPaused())
            {
                m_gameTimer.Stop();
            }
            else
            {
                m_gameTimer.Resume();
            }
            m_gameTimer.Tick();

            window->BeginFrame();
            {
                m_application->Update(m_gameTimer);
                m_application->Draw(m_gameTimer);
                m_platformProvider->Present();
            }
            window->EndFrame();

            m_inputController->SampleNewInput();
            do
            {
                window->PollEvents();
            } while (GameTimer::GetCurrentTime() - frameBeginTime < m_fpsCap);
        }

        m_application->Exit();
    }

    double Engine::GetCurrentTime() const
    {
        return m_gameTimer.GetCurrentTime();
    }

    GraphicsAPIType crystal::Engine::GetGraphicsAPIType() const
    {
        return m_platformProvider->GetGraphicsAPIType();
    }

    UIStateMachine* Engine::GetUIStateMachine() const
    {
        return ptr(m_pUIStateMachine);
    }

    void Engine::ReadConfig()
    {
        auto&& node = SJson::JsonConvert::Parse(File::ReadAllText("resources/engine.json"));
        m_initArgs = SJson::de_serialize<InitArgs>(node["Init"]);
        m_fpsCap = 1.0 / m_initArgs.FPSLimit;
    }
}