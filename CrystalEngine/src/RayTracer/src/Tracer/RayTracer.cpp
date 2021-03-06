#include "RayTracer.h"
#include <Engine.h>

#include "Tracer/Integrator/DirectLightingIntegrator.h"
#include "Tracer/Integrator/PathTracingIntegrator.h"
#include "Tracer/Integrator/VolumePTIntegrator.h"
#include "Tracer/Integrator/WhiteFurnaceIntegrator.h"
#include <Core/Sampling/Sampler/DefaultSampler.h>
#include <Core/Sampling/Sampler/StratifiedSampler.h>

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

        m_rayScene = std::make_shared<RayScene>(scene, renderProps.EnvironmentLight);

        int sqrtSPP = (int)std::sqrt(renderProps.SampleCount);
        /*auto sampler = std::make_shared<DefaultSampler>(renderProps.SampleCount);*/
        auto sampler = std::make_shared<StratifiedSampler>(Point2i(sqrtSPP, sqrtSPP), 12);
        m_integrator = std::make_shared<VolumePTIntegrator>(sampler, renderProps.NumOfThreads, 12);
        //m_integrator = std::make_shared<WhiteFurnaceIntegrator>(sampler, renderProps.NumOfThreads);

        m_integrator->Render(cptr(m_rayScene), camera, ptr(m_frameBuffer));
    }

    std::shared_ptr<unsigned char[]> RayTracer::GetTexture2D() const
    {
        return m_frameBuffer->GetImageDataRGBA8();
    }

    void RayTracer::Save()
    {
        while (!m_integrator->IsFinished());
        auto data = m_frameBuffer->GetImageDataRGBA8();
        stbi_write_png("output.png", m_frameBuffer->GetWidth(), m_frameBuffer->GetHeight(), 4, data.get(),
            4 * m_frameBuffer->GetWidth());
    }
}