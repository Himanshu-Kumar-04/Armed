#pragma once
#include "Armed/scene/scene.h"
#include <unordered_map>
#include <vector>


namespace Arm {
    struct AssetPack {
        std::string validationToken = "ARMED_ASSET_PACK";
        struct ComponentBlock {

            std::vector<std::string> componentsPresent;

            //Tag

            std::string tag;

            //Transform

            glm::vec3 position, rotation, scale;

            //Camera

            bool isPrimary;
            bool hasFixedAspectRatio;

            float orthographicSize;
            float orthographicNear, orthographicFar;

            float perspectiveVerticalFOV;
            float perspectiveNear, perspectiveFar;

            SceneCamera::ProjectionType projectionType;

            //Sprite

            glm::vec4 color;

            //Mesh
        };

        std::unordered_map<UUID, ComponentBlock> entityMap;
        std::unordered_map<std::string, std::vector<UUID>> sceneMap;
    };
}