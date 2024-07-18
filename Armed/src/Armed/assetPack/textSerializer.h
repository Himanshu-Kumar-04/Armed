#pragma once
#include "Armed/scene/scene.h"
#include "assetPack.h"

namespace Arm {
    class TextSerializer {
    public:
        static void serializeAssets(const std::string& filePath, AssetPack& assetPack);
        static bool deserializeAssets(const std::string& filePath, AssetPack& assetPack);
    };
}