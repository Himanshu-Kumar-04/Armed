#include "ArmPCH.h"
#include "sceneSerializer.h"
#include "entity.h"

namespace Arm {
    void SceneSerializer::serialize(AssetPack& assetPack, const Ref<Scene>& scene)
    {
        std::vector<UUID> entityList;
        for (auto id : scene->m_EntityLibrary)
            entityList.push_back(id.first);
        
        assetPack.sceneMap[scene->m_SceneName] = entityList;

        for (UUID _UUID : entityList) {
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
    }
    bool SceneSerializer::deserialize(AssetPack& assetPack, Ref<Scene>& scene)
    {
        if (!scene)
            return false;

        scene = CreateRef<Scene>(scene->m_SceneName);
        for (UUID _UUID : assetPack.sceneMap[scene->m_SceneName]) {
            auto& entityData = assetPack.entityMap[_UUID];
            Entity e = scene->createEntity(_UUID, entityData.tag);
            //Transform
            TransformComponent& tc = e.getComponent<TransformComponent>();
            tc.translation = entityData.position;
            tc.rotation    = entityData.rotation;
            tc.scale       = entityData.scale;

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
        return true;
    }
}