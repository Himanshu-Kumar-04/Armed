#pragma once
#include <filesystem>
#include "Armed/scene/scene.h"
#include <vector>

namespace Arm {
    class TextSerializer {
    public:
        TextSerializer(const std::filesystem::path& filePath);
        ~TextSerializer();
        void serializeAssets(const Ref<Scene> scene);
    private:
        
    private:
        std::filesystem::path m_FilePath;
    };
}