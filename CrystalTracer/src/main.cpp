#include <CrystalEngine/src/Engine.h>
#include "CrystalTracer.h"

using namespace crystal;

int main()
{
	Engine* engine = Engine::GetInstance();
	engine->Start(std::make_unique<CrystalTracer>());
	return 0;
}
