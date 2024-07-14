#pragma once
#include "scene.h"
#include "Armed/assetPack/assetPack.h"

namespace Arm {
    class SceneSerializer {
    public:
        SceneSerializer(const Ref<Scene>& scene)
            : m_Scene(scene) {}
        AssetPack serialize();
        bool deserialize();
    private:
        Ref<Scene> m_Scene;
    };
}