#include "ArmPCH.h"
#include "sceneSerializer.h"
#include "entity.h"

namespace Arm {
    AssetPack SceneSerializer::serialize()
    {
        AssetPack assetPack;
        std::vector<UUID> entityList;
        for (auto id : m_Scene->m_EntityLibrary)
            entityList.push_back(id.first);
        assetPack.sceneMap[m_Scene->getSceneName()] = entityList;

        for (UUID _UUID : entityList) {
            Entity entity(m_Scene->m_EntityLibrary[_UUID], m_Scene.get());
            AssetPack::ComponentBlock componentBlock;
            if (entity.hasComponent<TagComponent>()) {
                componentBlock.componentsPresent.push_back("Tag");
                componentBlock.tag                      = entity.getComponent<TagComponent>().tag;
            }
            if (entity.hasComponent<TransformComponent>()) {
                componentBlock.componentsPresent.push_back("Transform");
                componentBlock.position                 = entity.getComponent<TransformComponent>().translation;
                componentBlock.rotation                 = entity.getComponent<TransformComponent>().rotation;
                componentBlock.scale                    = entity.getComponent<TransformComponent>().scale;
            }
            if (entity.hasComponent<CameraComponent>()) {
                componentBlock.componentsPresent.push_back("Camera");
                componentBlock.isPrimary                = entity.getComponent<CameraComponent>().isPrimary;          
                componentBlock.hasFixedAspectRatio      = entity.getComponent<CameraComponent>().hasFixedAspectRatio;
                componentBlock.projectionType           = entity.getComponent<CameraComponent>().camera.getProjectionType();
                componentBlock.orthographicSize         = entity.getComponent<CameraComponent>().camera.getOrthographicSize();
                componentBlock.orthographicNear         = entity.getComponent<CameraComponent>().camera.getNearClip(Camera::ProjectionType::orthographic);
                componentBlock.orthographicFar          = entity.getComponent<CameraComponent>().camera.getFarClip(Camera::ProjectionType::orthographic);
                componentBlock.perspectiveVerticalFOV   = entity.getComponent<CameraComponent>().camera.getPerspectiveVerticalFOV();
                componentBlock.perspectiveNear          = entity.getComponent<CameraComponent>().camera.getNearClip(Camera::ProjectionType::perspective);
                componentBlock.perspectiveFar           = entity.getComponent<CameraComponent>().camera.getFarClip(Camera::ProjectionType::perspective);
            }
            if (entity.hasComponent<SpriteRendererComponent>()) {
                componentBlock.componentsPresent.push_back("Sprite");
                componentBlock.color = entity.getComponent<SpriteRendererComponent>().color;
            }
            if (entity.hasComponent<MeshComponent>()) {
                componentBlock.componentsPresent.push_back("Mesh");
            }
            assetPack.entityMap[_UUID] = componentBlock;
        }
        return assetPack;
    }
    bool SceneSerializer::deserialize()
    {
        return false;
    }
}