#include "ArmPCH.h"
#include "entity.h"

namespace Arm {
    Entity::Entity(entt::entity handle, Scene* scene)
        :m_EntityHandle(handle), m_Scene(scene)
    {

    }
}