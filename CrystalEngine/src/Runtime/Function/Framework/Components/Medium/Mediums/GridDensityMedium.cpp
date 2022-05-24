#include "HomogeneousMedium.h"
#include "../PhaseFunctions/HenyeyGreenstein.h"
#include <Core/Interaction/SurfaceInteraction.h>
#include "GridDensityMedium.h"

namespace crystal
{
    GridDensityMedium::GridDensityMedium(const Spectrum& sigmaA, const Spectrum& sigmaS,
        Float g, const Transform& worldToLocal, const Array3D& d)
        : m_sigmaA(sigmaA), m_sigmaS(sigmaS),
        m_Nx(d.Nx), m_Ny(d.Ny), m_Nz(d.Nz), m_worldToLocal(worldToLocal)
    {
        m_phaseFunction = std::make_shared<HenyeyGreenstein>(g);
        m_density = std::make_unique<Float[]>(m_Nx * m_Ny * m_Nz);

        memcpy(m_density.get(), d.Density, sizeof(Float) * m_Nx * m_Ny * m_Nz);

        Float maxDensity = 0.f;
        for (int i = 0; i < m_Nx * m_Ny * m_Nz; i++)
        {
            maxDensity = std::max(maxDensity, m_density[i]);
        }
        m_invMaxDensity = maxDensity;
    }

    GridDensityMedium::~GridDensityMedium()
    {}

    Spectrum GridDensityMedium::Tr(const Ray3f & rayWorld, Float t, Sampler * sampler) const
    {
        auto rayStartLocal = m_worldToLocal.GetMatrix() * Vector4f(rayWorld.Start(), 1.f);
        auto rayDirLocal = m_worldToLocal.GetRotation() * rayWorld.Dir();
        Ray3f rayLocal(rayStartLocal, rayDirLocal);

        Bound3f b(Point3f(0, 0, 0), Point3f(1, 1, 1));
        Float tMin, tMax;
        if (!RayBoxTest(rayLocal, b, tMin, tMax))
        {
            return Spectrum(1.f);
        }

        constexpr int SpectrumChannels = 3;
        Float u = sampler->Get1D() * SpectrumChannels;
        int channel = std::min((int)u, SpectrumChannels - 1);
        Spectrum sigmaT = m_sigmaA + m_sigmaS;

        //while (true)
        //{
        //    t -= std::log(1 - sampler->Get1D()) * m_invMaxDensity / sigmaT[channel];
        //    if (t >= tMax) break;
        //    if (Density(rayLocal.Eval(t)) * m_invMaxDensity > sampler->Get1D())
        //    {
        //        // Populate _mi_ with medium interaction information and return
        //        *mi = MediumInteraction(rWorld(t), -rWorld.d, rWorld.time, this,
        //                                phase);
        //        return sigma_s / sigma_t;
        //    }
        //}
        return Spectrum(0.f);
    }

    Spectrum GridDensityMedium::Sample(const Ray3f& rayWorld, Float t, Sampler* sampler, MediumInteractionInfo* mi) const
    {
        return Spectrum();
    }

    Float GridDensityMedium::Density(const Point3f& pos) const
    {
        // The reason for -0.5 is that the value is defined at the center of each voxel
        Point3f samples(pos.x * m_Nx - 0.5f, pos.y * m_Ny - 0.5f, pos.z * m_Nz - 0.5f);
        Point3i pi = Point3i(glm::floor(samples));
        Vector3f diff = Vector3f(samples.x - pi.x, samples.y - pi.y, samples.z - pi.z);

        Float dx00 = glm::mix(D(pi), D(pi + Point3i(1, 0, 0)), diff.x);
        Float dx10 = glm::mix(D(pi + Point3i(0, 1, 0)), D(pi + Point3i(1, 1, 0)), diff.x);
        Float dx01 = glm::mix(D(pi + Point3i(0, 0, 1)), D(pi + Point3i(1, 0, 1)), diff.x);
        Float dx11 = glm::mix(D(pi + Point3i(0, 1, 1)), D(pi + Point3i(1, 1, 1)), diff.x);

        Float d0 = glm::mix(dx00, dx10, diff.y);
        Float d1 = glm::mix(dx10, dx11, diff.y);
        return glm::mix(d0, d1, diff.z);
    }

    Float GridDensityMedium::D(const Point3i& pos) const
    {
        if (pos.x < 0 || pos.x >= m_Nx || pos.y < 0 || pos.y >= m_Ny || pos.z < 0 || pos.z >= m_Nz)
        {
            return 0.f;
        }
        return m_density[pos.z * m_Nx * m_Ny + pos.y * m_Nx + pos.x];
    }
}
