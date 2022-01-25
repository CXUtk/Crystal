#include "Engine.h"
#include "Core/Application.h"

namespace crystal
{
    Engine::Engine()
    {

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