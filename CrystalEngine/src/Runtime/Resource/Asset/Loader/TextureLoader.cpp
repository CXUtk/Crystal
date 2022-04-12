#include "TextureLoader.h"
#include <Engine.h>

namespace crystal
{
    std::shared_ptr<ITexture2D> TextureLoader::LoadTexture2D(const SJson::JsonNode& metaData, const path_type& curPath)
    {
        auto engine = Engine::GetInstance();
        auto graphicsDevice = engine->GetGraphicsDevice();

        auto target = metaData["target"].Get<std::string>();

        auto pathToTarget = curPath / target;

        Texture2DDescription texDesc = {};
        texDesc.Format = SRefl::EnumInfo<RenderFormat>::string_to_enum(metaData["format"].Get<std::string>());
        texDesc.MipmapLevels = 1;
        texDesc.Usage = BufferUsage::Immutable;

        return  graphicsDevice->CreateTexture2DFromFile(pathToTarget.string(), texDesc);
    }
}