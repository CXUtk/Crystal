#include "RayTracer.h"
#include <Engine.h>


namespace tracer
{
    RayTracer::RayTracer()
    {}

    RayTracer::~RayTracer()
    {}

    void RayTracer::RenderAsync(std::shared_ptr<Scene> scene, const RenderProperties & renderProps)
    {
        auto engine = Engine::GetInstance();
        auto device = engine->GetGraphicsDevice();

        int width = renderProps.FrameBufferSize.x;
        int height = renderProps.FrameBufferSize.y;

        if (!m_frameBuffer || (m_bufferOldSize != renderProps.FrameBufferSize))
        {
            m_frameBuffer = std::make_shared<FrameBuffer>(width, height);
        }
        m_rayScene = std::make_shared<RayScene>(scene);
    }
}