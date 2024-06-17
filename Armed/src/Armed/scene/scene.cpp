#include "ArmPCH.h"
#include "scene.h"
#include "entity.h"
#include "components.h"
#include "scriptableEntity.h"

#include "glm/glm.hpp"
#include "Armed/renderer/renderer2D.h"
#include "Armed/renderer/renderer.h"

namespace Arm {
    Scene::Scene(SceneType sceneType)
        :m_SceneType(sceneType)
    {
    }

    Scene::~Scene()
    {
    }

    void Scene::onUpdate(Timestep ts)
    {
        {
            m_Registry.view<NativeScriptComponent>().each([&](auto entity, auto& msc) {
                if (!msc.instance) {
                    msc.instance = msc.instiateScript();
                    msc.instance->m_Entity = Entity{ entity, this };
                    msc.instance->onCreate();
                }
                msc.instance->onUpdate(ts);
            });
        }

        Camera* mainCamera = nullptr;
        glm::mat4* cameraTransform = nullptr;
        {
            auto view = m_Registry.view<TransformComponent, CameraComponent>();
            for (auto entity : view) {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

                if (camera.isPrimary) {
                    mainCamera = &camera.camera;
                    cameraTransform = &transform.transform;
                    break;
                }
            }
        }

        if (mainCamera) {
            if (m_SceneType == SceneType::__2D__) {
                Renderer2D::beginScene(mainCamera->getProjection(), *cameraTransform);

                auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
                for (auto entity : group) {
                    auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                    Renderer2D::drawQuad(transform, sprite.color);
                }

                Renderer2D::endScene();
            }
            else {
                /*Renderer::beginScene(mainCamera->getProjection(), *cameraTransform);

                auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
                for (auto entity : group) {
                    auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                    Renderer::submit(......---------........---------......);
                }

                Renderer::endScene();*/
            }
        }
    }

    void Scene::onViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view) {
            auto& cameraComponent = view.get<CameraComponent>(entity);

            if (!cameraComponent.hasFixedAspectRatio)
                cameraComponent.camera.setViewportSize(width, height);
        }
    }

    Entity Scene::createEntity(const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };
        entity.addComponent<TransformComponent>();
        TagComponent& tag = entity.addComponent<TagComponent>();
        tag.tag = (name.empty()) ? "__entity__" : name;
        return entity;
    }

}