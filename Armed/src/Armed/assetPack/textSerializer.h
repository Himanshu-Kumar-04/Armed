#pragma once
#include <filesystem>
#include "Armed/scene/scene.h"
#include "assetPack.h"

namespace Arm {
    class TextSerializer {
    public:
        ~TextSerializer();
        void serializeAssets(const std::filesystem::path& filePath, const Ref<Scene> scene);
        bool deserializeAssets(const std::filesystem::path& filePath);
        AssetPack m_AssetPack;
    };
}