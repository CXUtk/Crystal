#pragma once
#include "../Medium.h"
#include "../PhaseFunctions/PhaseFunction.h"
namespace crystal
{
    class HomogeneousMedium : public Medium
    {
    public:
        HomogeneousMedium(const Spectrum& sigmaA, const Spectrum& sigmaS, Float g);
        virtual ~HomogeneousMedium() override;

        virtual Spectrum Tr(const Ray3f& ray, Float t, Sampler* sampler) const override;
        virtual Spectrum Sample(const Ray3f& ray, Float t, Sampler* sampler, MediumInteractionInfo* mi) const override;

    private:
        Spectrum    m_sigmaA{}, m_sigmaS{};

        std::shared_ptr<PhaseFunction> m_phaseFunction{};
    };
}