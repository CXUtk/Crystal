#include <Engine.h>
//#include <CrystalEngine/src/Tests/Integrated/OrbitControllerTest.h>
//#include <CrystalEngine/src/Tests/Integrated/TriangleTest.h>
//#include <CrystalEngine/src/Tests/Integrated/QuadTest.h>
//#include <CrystalEngine/src/Tests/Integrated/RenderTargetTest.h>
//#include <CrystalEngine/src/Core/Utils/Logger.h>
//#include <CrystalEngine/src/Core/Utils/MemoryPool.h>
#include <SJson/SJson.hpp>

#include "CrystalTracer.h"
using namespace crystal;
using namespace tracer;
//
//#include <Interfaces/FileSystem/File.h>
//#include <type_traits>

#include <thread>
#include <mutex>

#include <Core/Utils/FixedThreadPool.h>

int main(int argc, char** argv)
{
    FixedThreadPool pool(8);

    for (int i = 0; i < 16; i++)
    {
        pool.AddTask(FixedThreadPool::Task{
            [i]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                printf("Thread %d Finished!\n", i);
            } });
    }

    while (true)
    {

    }
    //Engine* engine = Engine::GetInstance();
    //auto uptr = std::make_unique<CrystalTracer>();
    //engine->Start(std::move(uptr));
    return 0;
}
