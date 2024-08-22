#include "ArmPCH.h"
#include "textSerializer.h"
#include "Armed/scene/sceneSerializer.h"
#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>
#include <fstream>
#include "Armed/core/log.h"

namespace YAML {

    template<>
    struct convert<glm::vec2> {
        static Node encode(const glm::vec2& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec2& rhs) {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs) {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4> {
        static Node encode(const glm::vec4& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs) {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

    template<>
    struct convert<Arm::UUID> {
        static Node encode(const Arm::UUID& uuid) {
            Node node;
            node.push_back((uint64_t)uuid);
            return node;
        }

        static bool decode(const Node& node, Arm::UUID& uuid) {
            uuid = node.as<uint64_t>();
            return true;
        }
    };
}

namespace Arm {
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& vec)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << vec.x << vec.y << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, glm::vec3& vec) {
        out << YAML::Flow;
        out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, glm::vec4& vec) {
        out << YAML::Flow;
        out << YAML::BeginSeq << vec.r << vec.g << vec.b << vec.a << YAML::EndSeq;
        return out;
    }

    void TextSerializer::serializeAssets(const std::string& filePath, AssetPack& assetPack)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "ValidationToken" << YAML::Value << assetPack.validationToken;

        /// Scenes and the entities they contain
        out << YAML::Key << "Scenes" << YAML::Value << YAML::BeginSeq;
        for (auto& pair : assetPack.sceneMap) {
            out << YAML::BeginMap;
            out << YAML::Key << "Name" << YAML::Value << pair.first;
            out << YAML::Key << "Data" << YAML::Value;
            out << YAML::Flow;
            out << YAML::BeginSeq;
            for (UUID _UUID : pair.second)
                out << _UUID;
            out << YAML::EndSeq;
            out << YAML::EndMap;
        }
        out << YAML::EndSeq;
        /// Entity Map
        out << YAML::Key << "Entities" << YAML::Value;

        out << YAML::BeginSeq;
        for (auto& entityData : assetPack.entityMap) {
            out << YAML::BeginMap;
            out << YAML::Key << "Entity" << YAML::Value << entityData.first;
            out << YAML::Key << "Components" << YAML::Flow << YAML::Value << entityData.second.componentsPresent;
            for (const auto& component : entityData.second.componentsPresent) {
                if (component == "Tag") {
                    out << YAML::Key << "TagComponent" << YAML::Value << YAML::BeginMap;
                    out << YAML::Key << "Tag" << YAML::Value << entityData.second.tag;
                    out << YAML::EndMap;
                }
                if (component == "Transform") {
                    out << YAML::Key << "TransformComponent" << YAML::Value;
                    out << YAML::BeginMap;
                    out << YAML::Key << "Translation" << YAML::Value << entityData.second.position;
                    out << YAML::Key << "Rotation" << YAML::Value << entityData.second.rotation;
                    out << YAML::Key << "Scale" << YAML::Value << entityData.second.scale;
                    out << YAML::EndMap;
                }
                if (component == "Camera") {
                    out << YAML::Key << "CameraComponent" << YAML::Value;
                    out << YAML::BeginMap;
                    out << YAML::Key << "IsPrimary" << YAML::Value << entityData.second.isPrimary;
                    out << YAML::Key << "HasFixedAspectRatio" << YAML::Value << entityData.second.hasFixedAspectRatio;
                    out << YAML::Key << "Camera" << YAML::Value;
                    out << YAML::BeginMap;
                    out << YAML::Key << "ProjectionType" << YAML::Value << (int)entityData.second.projectionType;

                    out << YAML::Key << "OrthographicSize" << YAML::Value << entityData.second.orthographicSize;
                    out << YAML::Key << "OrthographicNear" << YAML::Value << entityData.second.orthographicNear;
                    out << YAML::Key << "OrthographicFar" << YAML::Value << entityData.second.orthographicFar;

                    out << YAML::Key << "PerspectiveVerticalFOV" << YAML::Value << entityData.second.perspectiveVerticalFOV;
                    out << YAML::Key << "PerspectiveNear" << YAML::Value << entityData.second.perspectiveNear;
                    out << YAML::Key << "PerspectiveFar" << YAML::Value << entityData.second.perspectiveFar;
                    out << YAML::EndMap;
                    out << YAML::EndMap;
                }
                if (component == "Sprite") {
                    out << YAML::Key << "SpriteComponent" << YAML::Value << YAML::BeginMap;
                    out << YAML::Key << "Color" << YAML::Value << entityData.second.color;
                    out << YAML::EndMap;
                }
                if (component == "Mesh") {
                    out << YAML::Key << "MeshComponent" << YAML::Value << YAML::BeginMap;
                    out << YAML::EndMap;
                }
            }
            out << YAML::EndMap;
        }

        out << YAML::EndSeq;


        out << YAML::EndMap;

        std::ofstream stream(filePath);
        stream << out.c_str();
    }
    bool TextSerializer::deserializeAssets(const std::string& filePath, AssetPack& assetPack)
    {
        std::ifstream stream(filePath);

        YAML::Node data;
        try
        {
            data = YAML::LoadFile(filePath);
        }
        catch (YAML::ParserException e)
        {
            ARM_ERROR("Failed to load .armed file %s\n     {%s}", filePath.c_str(), e.what());
            return false;
        }

        if (data["ValidationToken"].as<std::string>() != "ARMED_ASSET_PACK" || !data["Scenes"])
            return false;
        //Reset assetPack struct to load a new file
        assetPack.entityMap.clear();
        assetPack.sceneMap.clear();

        auto scenes = data["Scenes"];

        if (scenes) {
            for (auto scene : scenes) {
                assetPack.sceneMap[scene["Name"].as<std::string>()] = scene["Data"].as<std::vector<UUID>>();
            }
        }
        auto entities = data["Entities"];
        for (auto entityData : entities) {
            AssetPack::ComponentBlock components;

            components.componentsPresent = entityData["Components"].as<std::vector<std::string>>();

            if (entityData["TagComponent"])
                components.tag = entityData["TagComponent"]["Tag"].as<std::string>();

            if (entityData["TransformComponent"]) {
                components.position = entityData["TransformComponent"]["Translation"].as<glm::vec3>();
                components.rotation = entityData["TransformComponent"]["Rotation"].as<glm::vec3>();
                components.scale = entityData["TransformComponent"]["Scale"].as<glm::vec3>();
            }

            if (entityData["CameraComponent"]) {
                components.isPrimary = entityData["CameraComponent"]["IsPrimary"].as<bool>();
                components.hasFixedAspectRatio = entityData["CameraComponent"]["HasFixedAspectRatio"].as<bool>();

                components.projectionType = (SceneCamera::ProjectionType)entityData["CameraComponent"]["Camera"]["ProjectionType"].as<int>();

                components.orthographicSize = entityData["CameraComponent"]["Camera"]["OrthographicSize"].as<float>();
                components.orthographicNear = entityData["CameraComponent"]["Camera"]["OrthographicNear"].as<float>();
                components.orthographicFar  = entityData["CameraComponent"]["Camera"]["OrthographicFar"].as<float>();

                components.perspectiveVerticalFOV = entityData["CameraComponent"]["Camera"]["PerspectiveVerticalFOV"].as<float>();
                components.perspectiveNear = entityData["CameraComponent"]["Camera"]["PerspectiveNear"].as<float>();
                components.perspectiveFar = entityData["CameraComponent"]["Camera"]["PerspectiveFar"].as<float>();
            }

            if (entityData["SpriteComponent"])
                components.color = entityData["SpriteComponent"]["Color"].as<glm::vec4>();

            if (entityData["MeshComponent"])
                continue;

            assetPack.entityMap[entityData["Entity"].as<UUID>()] = components;
        }
        return true;
    }
}