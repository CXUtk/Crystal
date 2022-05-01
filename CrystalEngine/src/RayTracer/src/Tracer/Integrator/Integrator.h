#pragma once
#include <Crystal.h>
#include <memory>
#include <Tracer/Core/RayScene.h>
#include <Tracer/Core/FrameBuffer.h>
#include <Function/Framework/Components/Camera/CameraComponent.h>

namespace tracer
{
    class Integrator
    {
    public:
        //static std::unique_ptr<Integrator> LoadIntegrator(JsonNode_CPTR pConfigNode,
        //    const config::ConfigInfo& configInfo);

        Integrator() {}

        virtual ~Integrator() = 0 {}
        virtual void Render(const RayScene* rayScene,
            const CameraComponent* camera, FrameBuffer* frameBuffer) = 0;
        virtual bool IsFinished() const = 0;
    };
}