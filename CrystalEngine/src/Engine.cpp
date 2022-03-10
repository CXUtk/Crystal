#include "Engine.h"
#include "Core/Application.h"

#include "Core/Utils/GameTimer.h"
#include "Core/Input/InputController.h"
#include "Core/Utils/Logger.h"
#include "Core/Render/RenderExports.h"
#include "Core/Asset/AssetManager.h"

#include "Platforms/PlatformFactory.h"
#include <Core/UI/UIExports.h>

namespace crystal
{
    Engine::Engine()
    {
        m_initArgs = {};
        m_initArgs.WindowWidth = 800;
        m_initArgs.WindowHeight = 600;
        m_initArgs.WindowResizable = false;
        m_initArgs.FPSLimit = 60;
        strcpy(m_initArgs.WindowTitle, "Test");

        m_fpsCap = 1.0 / m_initArgs.FPSLimit;

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
}