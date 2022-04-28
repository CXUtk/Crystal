#pragma once
#include <Crystal.h>
#include <glm/glm.hpp>

namespace crystal
{
    BIT_FLAGS_BEGIN(LightFlags, uint32_t)
    {
        None = 0,
        DeltaPosition = 1 << 0,
        DeltaDirection = 1 << 1,
        Area = 1 << 2,
        Infinite = 1 << 3
    };
    BIT_FLAGS_END(LightFlags);

    class Light
    {
    public:
        Light(LightFlags flags, int numSamples) : m_flags(flags), m_numSamples(numSamples) { }

        virtual ~Light() = 0 {}

        //virtual const crystal::IIntersectable* GetAttachedObject() const { return nullptr; }

        //virtual Spectrum Flux() const = 0;

        //virtual void Preprocess(const Scene* scene) const { }

        //// Get the radiance value from the light to the object surface hit point
        //virtual Spectrum Sample_Li(const SurfaceInfo& surface_w, const glm::vec2& sample,
        //    Point3f* endpoint, float* pdf) const = 0;

        //virtual float Pdf_Li(const SurfaceInfo& surface_w, const Vector3f& wi) const = 0;

        //// Infinite area light, get emitted radiance from direction
        //virtual Spectrum Le(const Vector3f& wi) const { return Spectrum(0.f); }

        //int GetNumSamples() const { return _numSamples; }

        //LightFlags GetFlags() const { return _flags; }

        bool IsDeltaLight() const
        {
            return (m_flags & (LightFlags::DeltaPosition | LightFlags::DeltaDirection));
        }

    private:
        int         m_numSamples{};
        LightFlags  m_flags{};
    };
}
