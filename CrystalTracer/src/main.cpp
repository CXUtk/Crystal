#include <CrystalEngine/src/Engine.h>
#include "CrystalTracer.h"

#include <CrystalEngine/src/Core/Utils/Logger.h>
using namespace crystal;

int main()
{
	Engine* engine = Engine::GetInstance();
	try
	{
		engine->Start(std::make_unique<tracer::CrystalTracer>());
	}
	catch (std::exception ex)
	{
		crystal::GlobalLogger::Log(SeverityLevel::Error, ex.what());
	}
	return 0;
}
