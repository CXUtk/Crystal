#include "Engine.h"
#include "Core/Application.h"
#include "Core/InitArgs.h"
#include "Core/Utils/GameTimer.h"
#include "Core/Input/InputController.h"

#ifdef CRYSTAL_USE_GLFW
#include <Platforms/GLFWPlatform.h>
#endif

namespace crystal
{
    Engine::Engine()
    {
        InitArgs args;
        args.WindowWidth = 800;
        args.WindowHeight = 600;
        args.WindowResizable = false;
        strcpy(args.WindowTitle, "Test");

#ifdef CRYSTAL_USE_GLFW
        _platformProvider = std::make_unique<GLFWProvider>(args);
#endif
        _gameTimer = std::make_unique<GameTimer>();
    }
    
    Engine::~Engine()
    {
    }

    IGameWindow* Engine::GetWindow() const
    {
        return _platformProvider->GetGameWindow();
    }

    void Engine::Start(std::unique_ptr<Application>&& application)
    {
        _application = std::move(application);
        _application->SetEngine(this);

        _application->Initialize();

        auto window = this->GetWindow();
        _inputController = std::make_unique<InputController>(window);

        double cappedElapsedTime = 1.0 / 60.0;
        double prevTime = _gameTimer->GetTimeFromGameStartInSeconds();
        double deltaTime = 0.0;
        while (!window->ShouldClose())
        {
            double curTime = _gameTimer->GetTimeFromGameStartInSeconds();
            deltaTime = curTime - prevTime;
            prevTime = curTime;

            window->BeginFrame();
            {
                _application->Update(deltaTime);

                _application->Draw(deltaTime);
            }
            window->EndFrame();

            do
            {
                window->PollEvents();
            } while (_gameTimer->GetTimeFromGameStartInSeconds() - prevTime < cappedElapsedTime);
        }

        _application->Exit();
    }
}