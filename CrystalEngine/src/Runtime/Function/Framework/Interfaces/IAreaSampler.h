#pragma once
#include <Function/Framework/Object/GameObject.h>
#include <Core/Interaction/SurfaceInteraction.h>

namespace crystal
{
    class IAreaSampler
    {
    public:
        virtual ~IAreaSampler() = 0 {};

        // Total surface area of given model
        virtual Float SurfaceArea() const = 0;

        // Use specific sampling method to sample a patch on the surface
        virtual SurfaceInfo SampleSurface(const Vector2f& sample) const = 0;

        // Pdf of a given surface patch in current surface sampling method
        virtual Float SurfacePdf(const SurfaceInfo& surface_w) const { return 1.f / SurfaceArea(); }

        // Pdf of a given surface patch in solid angle of shading point hemisphere
        virtual Float PdfLight(const SurfaceInfo& surface_w, const Vector3f& wi) const = 0;
    };
}