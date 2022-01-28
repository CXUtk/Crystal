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
        strcpy(args.WindowTitle, "Test");

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

        double cappedElapsedTime = 1.0 / 60.0;
        double frameBeginTime = 0.0;
        _gameTimer = std::make_unique<GameTimer>();
        while (!window->ShouldClose())
        {
            frameBeginTime = _gameTimer->GetCurrentTime();
            if (_application->Paused())
            {
                _gameTimer->Stop();
            }
            else
            {
                _gameTimer->Start();
            }
            _gameTimer->Tick();

            window->BeginFrame();
            {
                auto deltaTime = _gameTimer->GetDeltaTime();
                _application->Update(deltaTime);
                _application->Draw(deltaTime);
            }
            window->EndFrame();

            _inputController->SampleNewInput();
            do
            {
                window->PollEvents();
            } while (_gameTimer->GetCurrentTime() - frameBeginTime < cappedElapsedTime);
        }

        _application->Exit();
    }
}