#include "ArmPCH.h"
#include "scriptableEntity.h"
#include "components.h"
#include "entity.h"
#include "scene.h"
#include "sceneSerializer.h"

#include "glm/glm.hpp"
#include "Armed/renderer/renderer2D.h"
#include "Armed/renderer/renderer.h"

namespace Arm {
    Scene::Scene(const std::string& sceneName, SceneType sceneType)
        :m_SceneName(sceneName),m_SceneType(sceneType)
    {
        if (m_SceneType == SceneType::__2D__)
            Renderer2D::init();
        else
            Renderer::init();
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
                if(m_SceneState != SceneState::paused) msc.instance->onUpdate(ts);
            });
        }

        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform;
        {
            auto view = m_Registry.view<TransformComponent, CameraComponent>();
            for (auto entity : view) {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

                if (camera.isPrimary) {
                    mainCamera = &camera.camera;
                    cameraTransform = transform.getTransform();
                    break;
                }
            }
        }

        if (mainCamera) {
            if (m_SceneType == SceneType::__2D__) {
                Renderer2D::beginScene(mainCamera->getProjection(), cameraTransform);

                RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
                RenderCommand::clearColor();
                RenderCommand::enableDepthTest();

                auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
                for (auto entity : group) {
                    auto [tc, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                    Renderer2D::drawQuad(tc.getTransform(), sprite.color);
                }

                Renderer2D::endScene();
            }
            else if (m_SceneType == SceneType::__3D__) {
                Renderer::beginScene(mainCamera->getProjection(), cameraTransform);

                RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
                RenderCommand::clearColor();
                RenderCommand::enableDepthTest();

                auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
                for (auto entity : group) {
                    auto [tc, mc] = group.get<TransformComponent, MeshComponent>(entity);
                    Renderer::submit(tc.getTransform(), mc.mesh);
                }

                Renderer::endScene();
            }
        }
    }

    void Scene::onViewportResize(uint32_t width, uint32_t height)
    {

        if (m_ViewportWidth == width && m_ViewportHeight == height)
            return;

        m_ViewportWidth = width;
        m_ViewportHeight = height;

        // Resize our non-FixedAspectRatio cameras
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.hasFixedAspectRatio)
                cameraComponent.camera.setViewportSize(width, height);
        }
    }

    Entity Scene::createEntity(const std::string& name)
    {
        return createEntity(UUID(), name);
    }

    Entity Scene::createEntity(UUID _UUID, const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };
        entity.addComponent<IDComponent>(_UUID);
        m_EntityLibrary[_UUID] = entity;
        entity.addComponent<TransformComponent>();
        TagComponent& tag = entity.addComponent<TagComponent>();
        tag.tag = (name.empty()) ? "__entity__" : name;
        return entity;
    }

    void Scene::destroyEntity(Entity entity)
    {
        m_Registry.destroy(entity);
    }

    template<typename T>
    void Scene::onAddComponent(Entity& entity, T& component)
    {
        static_assert(false);
    }

    template<>
    void Scene::onAddComponent<IDComponent>(Entity& entity, IDComponent& component)
    {

    }

    template<>
    void Scene::onAddComponent<TagComponent>(Entity& entity, TagComponent& component)
    {

    }

    template<>
    void Scene::onAddComponent<TransformComponent>(Entity& entity, TransformComponent& component)
    {

    }

    template<>
    void Scene::onAddComponent<MeshComponent>(Entity& entity, MeshComponent& component)
    {
    }
    
    template<>
    void Scene::onAddComponent<CameraComponent>(Entity& entity, CameraComponent& component)
    {
        component.camera.setViewportSize(m_ViewportWidth, m_ViewportHeight);
    }

    template<>
    void Scene::onAddComponent<SpriteRendererComponent>(Entity& entity, SpriteRendererComponent& component)
    {

    }

    template<>
    void Scene::onAddComponent<NativeScriptComponent>(Entity& entity, NativeScriptComponent& component)
    {

    }
}