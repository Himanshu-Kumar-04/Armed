#pragma once
#include"ArmPCH.h"
#include "entt.hpp"
#include"Armed/core/timestep.h"

namespace Arm {
    enum class SceneType : uint8_t{
        __2D__ = 0,
        __3D__
    };
    class Entity;

    class Scene {
    public:
        Scene(SceneType sceneType = SceneType::__2D__);
        ~Scene();
        void onUpdate(Timestep ts);
        void onViewportResize(uint32_t width, uint32_t height);
        Entity createEntity(const std::string& name = std::string());

        //temp
        entt::registry& reg() { return m_Registry; }
    private:
        SceneType m_SceneType;
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0;
        uint32_t m_ViewportHeight = 0;
        friend class Entity;
        friend class SceneHierarchyPanal;
    };
}