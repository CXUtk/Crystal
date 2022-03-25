#include <CrystalEngine/src/Engine.h>
#include <CrystalEngine/src/Tests/Integrated/OrbitControllerTest.h>
#include <CrystalEngine/src/Tests/Integrated/TriangleTest.h>
#include <CrystalEngine/src/Tests/Integrated/QuadTest.h>
#include <CrystalEngine/src/Tests/Integrated/RenderTargetTest.h>
#include <CrystalEngine/src/Core/Utils/Logger.h>
#include <CrystalEngine/src/Core/Utils/MemoryPool.h>
#include <SJson/SJson.hpp>

#include "CrystalTracer.h"
using namespace crystal;
using namespace tracer;
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


    ImplicitListPool allocator;

    auto ptr1 = allocator.Malloc(7);
    auto ptr2 = allocator.Malloc(7);
    allocator.Free(ptr2);
    allocator.Free(ptr1);
    auto ptr3 = allocator.Malloc(15);
    //allocator.Free(ptr2);
    //auto ptr4 = allocator.Malloc(4);
    //allocator.Free(ptr1);
    allocator.Free(ptr3);
    for (int i = 0; i < 10; i++)
    {
        auto tracer = (CrystalTracer*)allocator.Malloc(sizeof(CrystalTracer));
        *tracer = CrystalTracer();
        allocator.Free(tracer);
    }

    auto uptr = std::make_unique<CrystalTracer>();
    engine->Start(std::move(uptr));
    return 0;
}
