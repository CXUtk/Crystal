#pragma once
#include <Crystal.h>
#include <memory>
#include <glm/glm.hpp>
#include <Core/Math/Geometry.h>
#include <Platform/RHI/Graphics/GraphicsCommon.h>
#include <Resource/Asset/Content/CPUTexture/CPUTexture2D.h>

#include "ComponentSettings.h"

namespace crystal
{
    class SurfaceInteraction;
    class Material
    {
    public:
        static std::shared_ptr<Material> CreateMaterial(const SJson::JsonNode& setting);

        Material() {}
        virtual ~Material() = 0 {};

        virtual void ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera = true) const = 0;
    };
}