#include "Material.h"

#include "Materials/LambertianMaterial.h"
#include "Materials/GlassMaterial.h"
#include "Materials/MirrorMaterial.h"
#include "Materials/PlasticMaterial.h"
#include "Materials/MetalMaterial.h"

#include <Engine.h>
#include <Resource/Asset/AssetManager.h>


namespace crystal
{
    std::shared_ptr<Material> crystal::Material::CreateMaterial(const SJson::JsonNode& setting)
    {
        auto assetManager = Engine::GetInstance()->GetAssetManager();
        MaterialType type = SRefl::EnumInfo<crystal::MaterialType>::string_to_enum(
            setting["Type"].Get<std::string>()
        );
        auto& params = setting["Data"];
        switch (type)
        {
        case crystal::MaterialType::Lambertian:
        {
            auto settings = SJson::de_serialize<LambertianMaterialSettings>(params);
            auto Kd = assetManager->LoadAsset<CPUTexture2D>(settings.Kd);
            return std::make_shared<LambertianMaterial>(Kd);
        }
        case crystal::MaterialType::Glass:
        {
            auto settings = SJson::de_serialize<GlassMaterialSettings>(params);
            auto Ks = assetManager->LoadAsset<CPUTexture2D>(settings.Ks);
            auto Tf = assetManager->LoadAsset<CPUTexture2D>(settings.Tf);
            Float IOR = settings.IOR;
            return std::make_shared<GlassMaterial>(Ks, Tf, IOR);
        }
        case crystal::MaterialType::Mirror:
        {
            auto settings = SJson::de_serialize<MirrorMaterialSettings>(params);
            auto Ks = assetManager->LoadAsset<CPUTexture2D>(settings.Ks);
            return std::make_shared<MirrorMaterial>(Ks);
        }
        case crystal::MaterialType::Plastic:
        {
            auto settings = SJson::de_serialize<PlasticMaterialSettings>(params);
            auto Kd = assetManager->LoadAsset<CPUTexture2D>(settings.Kd);
            auto Roughness = assetManager->LoadAsset<CPUTexture2D>(settings.Roughness);
            return std::make_shared<PlasticMaterial>(Kd, Roughness, settings.IOR);
        }
        case crystal::MaterialType::Metal:
        {
            auto settings = SJson::de_serialize<MetalMaterialSettings>(params);
            auto Kd = assetManager->LoadAsset<CPUTexture2D>(settings.Kd);
            auto Roughness = assetManager->LoadAsset<CPUTexture2D>(settings.Roughness);
            // Gold: Spectrum(0.181f, 0.608f, 1.562f), Spectrum(3.068f, 2.54f, 1.904f);
            return std::make_shared<MetalMaterial>(Kd, Roughness, settings.IOR, settings.K);
        }
        default:
            throw std::runtime_error("Not implemented");
            break;
        }
        return nullptr;
    }
}
