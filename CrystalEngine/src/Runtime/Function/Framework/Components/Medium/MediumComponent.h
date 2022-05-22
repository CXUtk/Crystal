#pragma once
#include <string>
#include <vector>
#include <memory>
#include <SJson/SJson.hpp>

#include "../Component.h"
#include "Medium.h"


namespace crystal
{
    class MediumComponent : public Component
    {
    public:
        MediumComponent(const SJson::JsonNode& setting);
        virtual ~MediumComponent() override;

        virtual void Initialize() override;
        virtual void Update(const GameTimer& gameTimer) override;
        virtual void Draw(const GameTimer& gameTimer) override;

        const MediumInterface& GetMediumInterface() const { return m_mediumInterface; }

    private:
        SJson::JsonNode                     m_setting{};
        MediumInterface                     m_mediumInterface{};
        std::shared_ptr<Medium>             m_mediumIn{}, m_mediumOut{};
    };
}

