#include <CrystalEngine/src/Engine.h>
#include <CrystalEngine/src/Tests/OrbitControllerTest.h>
#include <CrystalEngine/src/Tests/TriangleTest.h>
#include <CrystalEngine/src/Core/Utils/Logger.h>
using namespace crystal;

int main()
{
	Engine* engine = Engine::GetInstance();
	try
	{
		engine->Start(std::make_unique<OrbitControllerTest>());
	}
	catch (std::exception ex)
	{
		crystal::GlobalLogger::Log(SeverityLevel::Error, ex.what());
	}
	return 0;
}
