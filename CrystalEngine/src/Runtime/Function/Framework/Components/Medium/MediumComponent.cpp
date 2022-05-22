#include "MediumComponent.h"
#include "ComponentSettings.h"
#include "Mediums/HomogeneousMedium.h"

namespace crystal
{
    MediumComponent::MediumComponent(const SJson::JsonNode& setting)
        : m_setting(setting)
    {}

    MediumComponent::~MediumComponent()
    {}

    void MediumComponent::Initialize()
    {
        // Treat as null medium when no parameter was passed

            m_mediumIn = std::make_shared<HomogeneousMedium>(Spectrum(0.10014, 0.16503, 0.2468) * 6.f,
                Spectrum(8.9053e-05, 8.372e-05, 0) * 10.f, 0.7f);
            m_mediumInterface.Inside = m_mediumIn.get();
            return;
        
        MediumType type = SRefl::EnumInfo<crystal::MediumType>::string_to_enum(m_setting["Type"].Get<std::string>());
        auto& params = m_setting["Data"];
        switch (type)
        {
        case crystal::MediumType::Homogeneous:
        {
            throw std::runtime_error("Not implemented");
            break;
        }
        case crystal::MediumType::Grid:
        {
            throw std::runtime_error("Not implemented");
            // m_shape = std::make_shared<Triangle>(radius, transform);
            break;
        }
        default:
            throw std::runtime_error("Not implemented");
            break;
        }
    }

    void MediumComponent::Update(const GameTimer & gameTimer)
    {}

    void MediumComponent::Draw(const GameTimer & gameTimer)
    {}
}
