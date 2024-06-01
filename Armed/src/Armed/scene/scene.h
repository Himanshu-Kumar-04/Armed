#pragma once
#include"ArmPCH.h"
#include "entt.h"
#include"Armed/core/timestep.h"

namespace Arm {
    class Entity;

    class Scene {
    public:
        Scene();
        ~Scene();
        void onUpdate(Timestep ts);
        Entity createEntity(const std::string& name = std::string());

        //temp
        entt::registry& reg() { return m_Registry; }
    private:
        entt::registry m_Registry;

        friend class Entity;
    };
}