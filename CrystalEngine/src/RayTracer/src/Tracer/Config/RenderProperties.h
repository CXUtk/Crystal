#pragma once
#include <Core/Math/Geometry.h>
#include <Function/Framework/Components/Light/Lights/Light.h>

namespace tracer
{
    using namespace crystal;

    struct RenderProperties
    {
        Vector2i    FrameBufferSize{ 800, 600 };
        size_t      SampleCount{ 1 };
        size_t      NumOfThreads{ 1 };

        float       Gamma{ 2.2 };

        std::shared_ptr<Light> EnvironmentLight = nullptr;
    };
}