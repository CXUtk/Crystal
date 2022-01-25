#include <CrystalEngine/src/Engine.h>
#include "CrystalTracer.h"

using namespace crystal;

int main()
{
	CrystalTracer tracer(Engine::GetInstance());
	tracer.Run();
	return 0;
}
