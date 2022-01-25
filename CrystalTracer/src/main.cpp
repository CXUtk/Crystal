#include <CrystalEngine/src/Engine.h>
#include "CrystalTracer.h"

using namespace crystal;

int main()
{
	Engine* engine = Engine::GetInstance();
	engine->Start(std::make_unique<tracer::CrystalTracer>());
	return 0;
}
