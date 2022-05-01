#pragma once
#include <thread>
#include "Config/RenderProperties.h"
#include "Core/FrameBuffer.h"

#include <Function/Render/RenderExports.h>
#include <Function/Framework/Scene/Scene.h>

#include "Core/RayScene.h"
#include "Tracer/Integrator/Integrator.h"

namespace tracer
{
    class RayTracer
    {
    public:
        RayTracer();
        ~RayTracer();

        void RenderAsync(std::shared_ptr<Scene> scene,
            const RenderProperties& renderProps, const CameraComponent* camera);

        std::shared_ptr<unsigned char[]> GetTexture2D() const;

        void Save();

    private:
        std::shared_ptr<RayScene>       m_rayScene = nullptr;
        std::shared_ptr<FrameBuffer>    m_frameBuffer = nullptr;
        std::shared_ptr<Integrator>     m_integrator = nullptr;
        // std::shared_ptr<std::thread>    m_renderThread = nullptr;
        Vector2i                        m_bufferOldSize{};
    };
}