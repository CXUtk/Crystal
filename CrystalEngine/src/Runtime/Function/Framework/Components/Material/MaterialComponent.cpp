#include "MaterialComponent.h"
#include "Materials/LambertianMaterial.h"
#include "Materials/GlassMaterial.h"
#include "Materials/MirrorMaterial.h"
#include "Materials/PlasticMaterial.h"

#include <Engine.h>
#include <Resource/Asset/AssetManager.h>

namespace crystal
{

    MaterialComponent::MaterialComponent(std::shared_ptr<Material> material)
        : m_material(material)
    {}

    MaterialComponent::MaterialComponent(const SJson::JsonNode& setting)
        : m_setting(setting)
    {}

    MaterialComponent::~MaterialComponent()
    {}

    void MaterialComponent::Initialize()
    {
        if (m_setting.GetType() == SJson::ValueType::Null)
        {
            return;
        }
        auto assetManager = Engine::GetInstance()->GetAssetManager();

        MaterialType type = SRefl::EnumInfo<crystal::MaterialType>::string_to_enum(
            m_setting["Type"].Get<std::string>()
        );

        auto& params = m_setting["Data"];
        switch (type)
        {
        case crystal::MaterialType::Lambertian:
        {
            auto settings = SJson::de_serialize<LambertianMaterialSettings>(params);
            auto Kd = assetManager->LoadAsset<CPUTexture2D>(settings.Kd);
            m_material = std::make_shared<LambertianMaterial>(Kd);
            break;
        }
        case crystal::MaterialType::Glass:
        {
            auto settings = SJson::de_serialize<GlassMaterialSettings>(params);
            auto Ks = assetManager->LoadAsset<CPUTexture2D>(settings.Ks);
            auto Tf = assetManager->LoadAsset<CPUTexture2D>(settings.Ks);
            Float IOR = settings.IOR;
            m_material = std::make_shared<GlassMaterial>(Ks, Tf, IOR);
            break;
        }
        case crystal::MaterialType::Mirror:
        {
            auto settings = SJson::de_serialize<MirrorMaterialSettings>(params);
            auto Ks = assetManager->LoadAsset<CPUTexture2D>(settings.Ks);
            m_material = std::make_shared<MirrorMaterial>(Ks);
            break;
        }
        case crystal::MaterialType::Plastic:
        {
            auto settings = SJson::de_serialize<PlasticMaterialSettings>(params);
            auto Kd = assetManager->LoadAsset<CPUTexture2D>(settings.Kd);
            auto Roughness = assetManager->LoadAsset<CPUTexture2D>(settings.Roughness);
            m_material = std::make_shared<PlasticMaterial>(Kd, Roughness, settings.IOR);
            break;
        }
        default:
            throw std::runtime_error("Not implemented");
            break;
        }
    }

    void MaterialComponent::Update(const GameTimer & gameTimer)
    {}

    void MaterialComponent::Draw(const GameTimer & gameTimer)
    {}
}
