#include "TextureLoader.h"
#include <Engine.h>

namespace crystal
{
    std::shared_ptr<ITexture2D> TextureLoader::LoadTexture2D(const SJson::JsonNode& metaData, const path_type& curPath)
    {
        auto engine = Engine::GetInstance();
        auto graphicsDevice = engine->GetGraphicsDevice();
        auto path = curPath / path_type(metaData["path"].Get<std::string>());

        Texture2DDescription texDesc = {};
        texDesc.Format = RenderFormat::RGBA8ub;
        texDesc.MipmapLevels = 1;
        texDesc.Usage = BufferUsage::Immutable;

        return  graphicsDevice->CreateTexture2DFromFile(path.string(), texDesc);
    }
}