#pragma once
#include <Core/Math/Geometry.h>


namespace tracer
{
    using namespace crystal;
    struct RenderProperties
    {
        Vector2i    FrameBufferSize{800, 600};
        size_t      SampleCount{1};

        float       Gamma{ 2.2 };
    };
}