#include "ArmPCH.h"
#include "scene.h"
#include "entity.h"
#include "components.h"

#include "glm/glm.hpp"
#include "Armed/renderer/renderer2D.h"

namespace Arm {
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    void Scene::onUpdate(Timestep ts)
    {
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group) {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            Renderer2D::drawQuad(transform, sprite.color);
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