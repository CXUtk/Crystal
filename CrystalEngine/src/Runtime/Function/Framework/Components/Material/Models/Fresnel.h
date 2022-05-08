#pragma once
#include <glm/glm.hpp>
#include <Crystal.h>
#include <Core/Math/Geometry.h>
#include <Platform/RHI/Graphics/GraphicsCommon.h>
#include <complex>

namespace crystal
{
    class Fresnel
    {
    public:
        virtual ~Fresnel() = 0 {};
        virtual Spectrum Eval(float cosThetaI) const = 0;
    };

    class FresnelNoOp : public Fresnel
    {
    public:
        FresnelNoOp() {}
        ~FresnelNoOp() override {}
        Spectrum Eval(float cosThetaI) const override { return glm::vec3(1.f); }
    };

    class FresnelFullTransmission : public Fresnel
    {
    public:
        FresnelFullTransmission() {}
        ~FresnelFullTransmission() override {}
        Spectrum Eval(float cosThetaI) const override { return glm::vec3(0.f); }
    };



    class FresnelDielectric : public Fresnel
    {
    public:
        FresnelDielectric(Float etaA, Float etaB) : m_etaA(etaA), m_etaB(etaB) {}
        ~FresnelDielectric() override {}
        Spectrum Eval(float cosThetaI) const override;

    private:
        Float m_etaA{}, m_etaB{};
    };

    class FresnelConductor : public Fresnel
    {
    public:
        FresnelConductor(const Spectrum& etaI, const Spectrum& etaT, const Spectrum& k)
            : m_etaI(etaI), m_etaT(etaT), m_K(k) {}
        ~FresnelConductor() override {}
        Spectrum Eval(float cosThetaI) const override;

    private:
        Spectrum m_etaI{}, m_etaT{};
        Spectrum m_K{};
    };

    class FresnelSchlick : public Fresnel
    {
    public:
        FresnelSchlick(Spectrum F0) : m_f0(F0) {}
        ~FresnelSchlick() override {}
        Spectrum Eval(float cosThetaI) const override;

    private:
        Spectrum m_f0{};
    };
}