#pragma once
#include <Interfaces/Graphics/GraphicsCommon.h>
#include <Core/Utils/Geometry.h>

namespace crystal
{
    class SpriteBatch;
    class GeometryRenderer;

    struct RenderPayload
    {
        SpriteBatch*                            SpriteBatch = nullptr;
        GeometryRenderer*                       GeometryRenderer = nullptr;
        std::shared_ptr<IPipelineStateObject>   PSO = nullptr;
    };
}