#pragma once

#include "scene.h"
#include "entt.h"

namespace Arm {

    class Entity {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity& other) = default;

        template<typename T, typename... Args>
        T& addComponent(Args&&... args) {
            ARM_ASSERT(!hasComponent<T>(), "m_Entity already has component");
            return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& getComponent() {
            ARM_ASSERT(hasComponent<T>(), "m_Entity does not have component");
            return m_Scene->m_Registry.get()<T>(m_EntityHandle);
        }

        template<typename T>
        bool hasComponent() {
            return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
        }

        template<typename T>
        void removeComponent() {
            m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

        operator bool() const { return (uint32_t)m_EntityHandle != 0; }
    private:
        entt::entity m_EntityHandle{ 0 };
        Scene* m_Scene = nullptr;
    };
}