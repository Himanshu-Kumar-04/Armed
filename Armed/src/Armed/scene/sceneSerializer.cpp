#include "ArmPCH.h"
#include "sceneSerializer.h"
#include "entity.h"

namespace Arm {
    void SceneSerializer::serialize(AssetPack& assetPack, const Ref<Scene> scene)
    {
        std::vector<UUID> entityList;
        for (auto id : scene->m_EntityLibrary)
            entityList.push_back(id.first);
        
        assetPack.sceneMap[scene->m_SceneName] = entityList;

        for (UUID _UUID : assetPack.sceneMap[scene->m_SceneName]) {
            Entity entity(scene->m_EntityLibrary[_UUID], scene.get());
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
                componentBlock.orthographicNear         = entity.getComponent<CameraComponent>().camera.getNearClip(SceneCamera::ProjectionType::orthographic);
                componentBlock.orthographicFar          = entity.getComponent<CameraComponent>().camera.getFarClip(SceneCamera::ProjectionType::orthographic);
                componentBlock.perspectiveVerticalFOV   = entity.getComponent<CameraComponent>().camera.getPerspectiveVerticalFOV();
                componentBlock.perspectiveNear          = entity.getComponent<CameraComponent>().camera.getNearClip(SceneCamera::ProjectionType::perspective);
                componentBlock.perspectiveFar           = entity.getComponent<CameraComponent>().camera.getFarClip(SceneCamera::ProjectionType::perspective);
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
    }
    bool SceneSerializer::deserialize(AssetPack& assetPack, Ref<Scene> scene)
    {
        if (!scene)
            return false;

        for (auto pair : scene->m_EntityLibrary) {
            if (std::find(assetPack.sceneMap[scene->getSceneName()].begin(), assetPack.sceneMap[scene->getSceneName()].end(),pair.first) == assetPack.sceneMap[scene->getSceneName()].end())
                scene->destroyEntityWithUUID(pair.first);
        }

        for (UUID _UUID : assetPack.sceneMap[scene->m_SceneName]) {
            Entity e;
            auto& entityData = assetPack.entityMap[_UUID];
            //entity is deleated
            if (scene->m_EntityLibrary.find(_UUID) == scene->m_EntityLibrary.end()) {
                e = scene->createEntity(_UUID, entityData.tag);
                //Camera
                for (std::string& component : entityData.componentsPresent) {
                    if (component == "Camera") {
                        CameraComponent& cc = e.addComponent<CameraComponent>();
                        cc.isPrimary = entityData.isPrimary;
                        cc.hasFixedAspectRatio = entityData.hasFixedAspectRatio;
                        cc.camera.setPerspective(entityData.perspectiveVerticalFOV, entityData.perspectiveNear, entityData.perspectiveFar);
                        cc.camera.setOrthographic(entityData.orthographicSize, entityData.orthographicNear, entityData.orthographicFar);
                        cc.camera.setProjectionType(entityData.projectionType);
                    }
                    if (component == "Sprite") {
                        e.addComponent<SpriteRendererComponent>(entityData.color);
                    }
                    if (component == "Mesh") {
                        e.addComponent<MeshComponent>();
                    }
                }
            }
            else {
                e = scene->getEntityWithUUID(_UUID);
                //Camera
                for (std::string& component : entityData.componentsPresent) {
                    if (component == "Camera") {
                        CameraComponent& cc = e.getComponent<CameraComponent>();
                        cc.isPrimary = entityData.isPrimary;
                        cc.hasFixedAspectRatio = entityData.hasFixedAspectRatio;
                        cc.camera.setPerspective(entityData.perspectiveVerticalFOV, entityData.perspectiveNear, entityData.perspectiveFar);
                        cc.camera.setOrthographic(entityData.orthographicSize, entityData.orthographicNear, entityData.orthographicFar);
                        cc.camera.setProjectionType(entityData.projectionType);
                    }
                    if (component == "Sprite") {
                        e.getComponent<SpriteRendererComponent>().color = entityData.color;
                    }
                    if (component == "Mesh") {
                        e.getComponent<MeshComponent>();
                    }
                }
            }
            //there are more entities then in asset pack
            //TODO: entity is modified
            //TODO: entity is unchanged

            //Transform
            TransformComponent& tc = e.getComponent<TransformComponent>();
            tc.translation = entityData.position;
            tc.rotation    = entityData.rotation;
            tc.scale       = entityData.scale;

            
        }
        return true;
    }
}