#pragma once
#include <memory>
#include "Platform/RHI/Interfaces.h"
#include "Platform/RHI/FileSystem/File.h"
#include "Platform/RHI/FileSystem/FSCommon.h"
#include <SJson/SJson.hpp>

#include "Resource/Asset/AssetMeta/AssetMetaInfo.h"
#include "Function/Framework/Components/Mesh/Mesh.h"

namespace crystal
{
    class MeshLoader
    {
    public:
        static std::shared_ptr<Mesh> LoadOneMeshFromObj(const SJson::JsonNode& metaInfo,
            const path_type& curPath, int index = 0);
    };
}