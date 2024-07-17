#pragma once
#include "Armed/scene/scene.h"
#include <map>
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

        std::map<UUID, ComponentBlock> entityMap;
        std::map<std::string, std::vector<UUID>> sceneMap;
    };
}