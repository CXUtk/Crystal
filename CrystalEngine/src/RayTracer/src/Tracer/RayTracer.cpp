#include "RayTracer.h"
#include <Engine.h>

#include "Tracer/Integrator/DirectLightingIntegrator.h"
#include "Tracer/Sampler/DefaultSampler.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stbi/stb_image_write.h>

namespace tracer
{
    RayTracer::RayTracer()
    {}

    RayTracer::~RayTracer()
    {}

    void RayTracer::RenderAsync(std::shared_ptr<Scene> scene, const RenderProperties& renderProps,
        const CameraComponent* camera)
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

        auto sampler = std::make_shared<DefaultSampler>(renderProps.SampleCount);
        m_integrator = std::make_shared<DirectLightingIntegrator>(sampler, 1, 10);

        m_renderThread = std::make_shared<std::thread>([=]() {
            m_integrator->Render(cptr(m_rayScene), camera, ptr(m_frameBuffer));
        });
        
    }

    std::shared_ptr<unsigned char[]> RayTracer::GetTexture2D() const
    {
        return m_frameBuffer->GetImageDataRGBA8();
    }

    void RayTracer::Save()
    {
        m_renderThread->join();
        auto data = m_frameBuffer->GetImageDataRGBA8();
        stbi_write_png("output.png", m_frameBuffer->GetWidth(), m_frameBuffer->GetHeight(), 4, data.get(),
            4 * m_frameBuffer->GetWidth());
    }
}