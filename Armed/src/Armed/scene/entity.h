#pragma once
#include "scene.h"
#include "components.h"
#include "entt.hpp"


namespace Arm {

    class Entity {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity& other) = default;

        template<typename T, typename... Args>
        T& addComponent(Args&&... args) {
            ARM_ASSERT(!hasComponent<T>(), "m_Entity already has component");
            T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
            m_Scene->onAddComponent<T>(*this, component);
            return component;
        }

        template<typename T>
        T& getComponent() {
            ARM_ASSERT(hasComponent<T>(), "m_Entity does not have component");
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template<typename T>
        bool hasComponent() {
            return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
        }

        template<typename T>
        void removeComponent() {
            m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

        operator bool() const { return (uint32_t)m_EntityHandle != entt::null; }
        operator entt::entity() const { return m_EntityHandle; }
        operator uint32_t() const { return (uint32_t)m_EntityHandle; }

        bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
        bool operator!=(const Entity& other) const { return !(*this == other); }
    private:
        entt::entity m_EntityHandle{ entt::null };
        Scene* m_Scene = nullptr;
    };
}