#include <Engine.h>
#include "CrystalTracer.h"
#include "ExampleApp/OrbitControllerTest.h"
#include "ExampleApp/TreeTest.h"

using namespace crystal;
int main(int argc, char** argv)
{
    try
    {
        Engine* engine = Engine::GetInstance();
        auto uptr = std::make_unique<OrbitControllerTest>();
        engine->Start(std::move(uptr));
    }
    catch (std::exception& e)
    {
        printf("%s\n", e.what());
        int x;
        scanf("%d", &x);
    }
    return 0;
}
