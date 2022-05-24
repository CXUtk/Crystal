#pragma once
#include "../Medium.h"
#include "../PhaseFunctions/PhaseFunction.h"
namespace crystal
{
    struct Array3D 
    {
        size_t Nx, Ny, Nz;
        const Float* Density;
    };

    class GridDensityMedium : public Medium
    {
    public:
        GridDensityMedium(const Spectrum& sigmaA, const Spectrum& sigmaS, Float g,
            const Transform& worldToLocal, const Array3D& d);
        virtual ~GridDensityMedium() override;

        virtual Spectrum Tr(const Ray3f& rayWorld, Float t, Sampler* sampler) const override;
        virtual Spectrum Sample(const Ray3f& rayWorld, Float t, Sampler* sampler, MediumInteractionInfo* mi) const override;
        virtual const PhaseFunction* GetPhaseFunction() const override { return cptr(m_phaseFunction); }

        Float Density(const Point3f& pos) const;

    private:
        Spectrum    m_sigmaA{}, m_sigmaS{};
        std::shared_ptr<PhaseFunction> m_phaseFunction{};

        size_t      m_Nx{}, m_Ny{}, m_Nz{};
        Transform   m_worldToLocal{};
        std::unique_ptr<Float[]> m_density{};

        Float       m_invMaxDensity{};

        Float D(const Point3i& pos) const;
    };
}