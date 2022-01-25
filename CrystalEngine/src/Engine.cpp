#include "Engine.h"
#include "Core/Application.h"
#include "Core/InitArgs.h"

#ifdef CRYSTAL_USE_GLFW
#include <Platforms/GLFW/GLFWPlatform.h>
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

    }
    
    Engine::~Engine()
    {
    }

    void Engine::Start(std::unique_ptr<Application>&& application)
    {
        _application = std::move(application);
        _application->SetEngine(this);

        _application->Initialize();


    }
}