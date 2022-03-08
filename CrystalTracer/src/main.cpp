#include <CrystalEngine/src/Engine.h>
#include <CrystalEngine/src/Tests/Integrated/OrbitControllerTest.h>
#include <CrystalEngine/src/Tests/Integrated/TriangleTest.h>
#include <CrystalEngine/src/Tests/Integrated/QuadTest.h>
#include <CrystalEngine/src/Tests/Integrated/UITest.h>
#include <CrystalEngine/src/Tests/Integrated/RenderTargetTest.h>
#include <CrystalEngine/src/Core/Utils/Logger.h>
#include <SJson/SJson.hpp>
using namespace crystal;
//
//#include <Interfaces/FileSystem/File.h>
//#include <type_traits>

int main()
{
	//fs::path path("C:\\Windows\\System32\\啊啊啊.txt");
	//
	//std::cout << path.lexically_normal().string() << std::endl;

	//path = path.lexically_normal();

	//std::cout << "Root Name: " << path.root_name().string() << std::endl;
	//std::cout << "Root Directory: " << path.root_directory().string() << std::endl;
	//std::cout << "Root Path: " << path.root_path().string() << std::endl;
	//std::cout << "Relative Path: " << path.relative_path().string() << std::endl;
	//std::cout << "Stem Path: " << path.stem().string() << std::endl;
	//std::cout << "Is Absolute: " << path.is_absolute() << std::endl;
	//std::cout << "Is Relative: " << path.is_relative() << std::endl;

	//path /= "a";
	//printf("%s\n", path.string().c_str());

	//std::string s = "123abc啊哈哈哈";
	//for (auto c : s)
	//{
	//	printf("%02X ", (unsigned char)c);
	//}
	//printf("\n");
	//File::WriteAllText("test.txt", s);
	//auto str = File::ReadAllText("test.txt");

	//std::cout << str << std::endl;
	Engine* engine = Engine::GetInstance();
	//try
	//{
	//	engine->Start(std::make_unique<UITest>());
	//}
	////catch (std::exception ex)
	////{
	////	crystal::GlobalLogger::Log(SeverityLevel::Error, ex.what());
	////}
    engine->Start(std::make_unique<UITest>());
	return 0;
}
