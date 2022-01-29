#include "Engine.h"
#include "Core/Application.h"
#include "Core/InitArgs.h"
#include "Core/Utils/GameTimer.h"
#include "Core/Input/InputController.h"
#include "Core/Utils/Logger.h"
#include "Core/Platform/Platforms.h"
#include "Platforms/PlatformFactory.h"

namespace crystal
{
    Engine::Engine()
    {
        InitArgs args;
        args.WindowWidth = 800;
        args.WindowHeight = 600;
        args.WindowResizable = false;
        args.FPSLimit = 60;
        strcpy(args.WindowTitle, "Test");

        m_fpsCap = 1.0 / args.FPSLimit;

        GlobalLogger::Log(SeverityLevel::Debug, "Engine Construct");

        _platformProvider = PlatformFactory::GetPlatformProvider(args);
    }
    
    Engine::~Engine()
    {
        GlobalLogger::Log(SeverityLevel::Debug, "Engine Destruct");
    }

    IGameWindow* Engine::GetWindow() const
    {
        return _platformProvider->GetGameWindow();
    }

    InputController* Engine::GetInputController() const
    {
        return ptr(_inputController);
    }

    void Engine::Start(std::unique_ptr<Application>&& application)
    {
        _application = std::move(application);
        _application->SetEngine(this);

        _application->Initialize();

        auto window = this->GetWindow();
        _inputController = std::make_unique<InputController>(window);

        m_gameTimer.Start();
        double frameBeginTime = 0.0;
        while (!window->ShouldClose())
        {
            frameBeginTime = GameTimer::GetCurrentTime();

            m_gameTimer.Sample();
            if (_application->Paused())
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
                _application->Update(m_gameTimer);
                _application->Draw(m_gameTimer);
            }
            window->EndFrame();

            _inputController->SampleNewInput();
            do
            {
                window->PollEvents();
            } while (GameTimer::GetCurrentTime() - frameBeginTime < m_fpsCap);
        }

        _application->Exit();
    }
}