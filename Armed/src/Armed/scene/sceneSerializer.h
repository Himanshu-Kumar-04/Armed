#pragma once
#include "scene.h"
#include "Armed/assetPack/assetPack.h"

namespace Arm {
    class SceneSerializer {
    public:
        static void serialize(AssetPack& assetPack, const Ref<Scene>& scene);
        static bool deserialize(AssetPack& assetPack);
    };
}