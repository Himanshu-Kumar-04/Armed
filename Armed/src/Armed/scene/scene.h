#pragma once
#include "components.h"

#include"ArmPCH.h"
#include "entt.hpp"
#include"Armed/core/timestep.h"
#include"glm/glm.hpp"

namespace Arm {
    
    class Entity;

    class Scene {
    public:
        enum class SceneType : uint8_t {
            __2D__ = 0,
            __3D__
        };
        enum class SceneState : uint8_t {
            paused = 0,
            running
        };
    public:
        Scene(SceneType sceneType = SceneType::__2D__);
        ~Scene();

        void onUpdate(Timestep ts);
        void onViewportResize(uint32_t width, uint32_t height);
        
        SceneState getSceneState(SceneState ) const { return m_SceneState; }
        void setSceneState(SceneState state) { m_SceneState = state; }

        //Create and Destroy Entities
        void destroyEntity(Entity entity);
        Entity createEntity(const std::string& name = std::string());

        bool isEntityInView(Camera& camera, const glm::mat4& ct, const glm::mat4& et);
        entt::registry& reg() { return m_Registry; }

    private:
        template<typename T>
        void onAddComponent(Entity& entity, T& component);

    private:
        SceneType m_SceneType;
        SceneState m_SceneState;
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 1;
        uint32_t m_ViewportHeight = 1;
        friend class Entity;
        friend class SceneHierarchyPanal;
    };
    
}