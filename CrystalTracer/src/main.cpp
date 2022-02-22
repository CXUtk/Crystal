#include <CrystalEngine/src/Engine.h>
#include <CrystalEngine/src/Tests/Integrated/OrbitControllerTest.h>
#include <CrystalEngine/src/Tests/Integrated/TriangleTest.h>
#include <CrystalEngine/src/Tests/Integrated/QuadTest.h>
#include <CrystalEngine/src/Tests/Integrated/RenderTargetTest.h>
#include <CrystalEngine/src/Core/Utils/Logger.h>
#include <SJson/SJson.hpp>
using namespace crystal;

int main()
{
	Engine* engine = Engine::GetInstance();
	try
	{
		engine->Start(std::make_unique<QuadTest>());
	}
	catch (std::exception ex)
	{
		crystal::GlobalLogger::Log(SeverityLevel::Error, ex.what());
	}
	return 0;
}
