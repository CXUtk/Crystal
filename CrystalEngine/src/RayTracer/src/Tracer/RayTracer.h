#pragma once
#include "Config/RenderProperties.h"
#include "Core/FrameBuffer.h"

#include <Function/Render/RenderExports.h>
#include <Function/Framework/Scene/Scene.h>
#include "Core/RayScene.h"

namespace tracer
{
    class RayTracer
    {
    public:
        RayTracer();
        ~RayTracer();

        void RenderAsync(std::shared_ptr<Scene> scene, const RenderProperties& renderProps);

    private:
        std::shared_ptr<RayScene>       m_rayScene = nullptr;
        std::shared_ptr<FrameBuffer>    m_frameBuffer = nullptr;
        Vector2i                        m_bufferOldSize{};
    };
}