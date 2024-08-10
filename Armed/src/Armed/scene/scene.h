#pragma once
#include "components.h"
#include <unordered_map>
#include "entt.hpp"
#include "Armed/core/timestep.h"
#include "glm/glm.hpp"

namespace Arm {
    
    class Entity;

    class Scene {
    public:
        Scene(const std::string& sceneName = "Untitled");
        ~Scene();

        void onUpdate(Timestep ts);
        void onViewportResize(uint32_t width, uint32_t height);

        std::string& getSceneName() { return m_SceneName; }
        void setSceneName(const std::string& sceneName) { m_SceneName = sceneName; }

        //Create and Destroy Entities
        void destroyEntity(Entity entity);
        void destroyEntityWithUUID(UUID _UUID);

        Entity getPrimaryCameraEntity();
        Entity getEntityWithName(std::string& name);
        Entity getEntityWithUUID(UUID _UUID);

        Entity createEntity(const std::string& name = std::string());
        Entity createEntity(UUID _UUID, const std::string& name = std::string());

        entt::registry& reg() { return m_Registry; }

    private:
        template<typename T>
        void onAddComponent(Entity& entity, T& component);

    private:
        std::string m_SceneName;
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 1;
        uint32_t m_ViewportHeight = 1;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanal;

        std::unordered_map<UUID, entt::entity> m_EntityLibrary;
    };
    
}