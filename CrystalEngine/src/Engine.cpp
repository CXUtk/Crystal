#include "Engine.h"
#include "Core/Application.h"
#include "Core/InitArgs.h"
#include "Core/Utils/GameTimer.h"
#include "Core/Input/InputController.h"
#include "Core/Utils/Logger.h"
#include "Core/Render/SpriteBatch.h"
#include "Core/Platform/Platforms.h"
#include "Platforms/PlatformFactory.h"

#include <Core/Platform/Graphics/GraphicsCommon.h>

namespace crystal
{
    Engine::Engine()
    {
        InitArgs args{};
        args.WindowWidth = 800;
        args.WindowHeight = 600;
        args.WindowResizable = false;
        args.FPSLimit = 60;
        strcpy(args.WindowTitle, "Test");

        m_fpsCap = 1.0 / args.FPSLimit;

        GlobalLogger::Log(SeverityLevel::Debug, "Engine Construct");

        InitGraphicsCommons();
        m_platformProvider = PlatformFactory::GetPlatformProvider(args);

        m_spriteBatch = std::make_unique<SpriteBatch>(GetGraphicsDevice());
    }
    
    Engine::~Engine()
    {
        m_application.reset();
        m_inputController.reset();
        m_platformProvider.reset();
        GlobalLogger::Log(SeverityLevel::Debug, "Engine Destruct");
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

    SpriteBatch* Engine::GetSpriteBatch() const
    {
        return ptr(m_spriteBatch);
    }
    
    //IGraphicsDevice* Engine::GetGraphicsDevice() const
    //{
    //    return m_platformProvider->GetGraphicsDevice();
    //}

    void Engine::Start(std::unique_ptr<Application>&& application)
    {
        m_application = std::move(application);
        m_application->SetEngine(this);

        m_application->Initialize();

        auto window = this->GetWindow();
        auto graphicsDevice = m_platformProvider->GetGraphicsDevice();
        m_inputController = std::make_unique<InputController>(window);

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
                graphicsDevice->Present();
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
}