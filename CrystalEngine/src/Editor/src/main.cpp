#include <Engine.h>
#include "CrystalTracer.h"

using namespace crystal;
int main(int argc, char** argv)
{
    Engine* engine = Engine::GetInstance();
    auto uptr = std::make_unique<tracer::CrystalTracer>();
    engine->Start(std::move(uptr));
    return 0;
}
