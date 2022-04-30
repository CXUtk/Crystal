#pragma once
#include <Crystal.h>
#include <memory>
#include <glm/glm.hpp>
#include <Core/Math/Geometry.h>
#include <Platform/RHI/Graphics/GraphicsCommon.h>

namespace crystal
{
    class SurfaceInteraction;
    class Material
    {
    public:
        // static std::shared_ptr<Material> CreateMaterial(JsonNode_CPTR pNode, const Scene* scene);

        Material() {}
        virtual ~Material() = 0 {};

        virtual void ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera = true) const = 0;
    };
}