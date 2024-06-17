#include "sceneHierarchyPanal.h"
#include "glm/gtc/type_ptr.hpp"
#include<ImGui/imgui.h>
namespace Arm {
    SceneHierarchyPanal::SceneHierarchyPanal(const Ref<Scene>& context)
    {
        SetContext(context);
    }
    void SceneHierarchyPanal::SetContext(const Ref<Scene>& context)
    {
        m_Context = context;
    }
    void SceneHierarchyPanal::onImGuiRender()
    {
        ImGui::Begin("Scene Hierarchy");
        auto view = m_Context->m_Registry.view<TagComponent>();
        for (auto entityHandle : view) {
            Entity entity(entityHandle, m_Context.get());
            drawEntityNode(entity);
        }
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectionContext = {};
        ImGui::End();

        ImGui::Begin("Properties");
        if (m_SelectionContext)
            showComponent(m_SelectionContext);
        ImGui::End();
    }
    void SceneHierarchyPanal::drawEntityNode(Entity& entity)
    {
        auto& tag = entity.getComponent<TagComponent>().tag;
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0);
        bool opened = ImGui::TreeNodeEx((const void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
        if (ImGui::IsItemClicked())
            m_SelectionContext = entity;
        if (opened) {
            ImGui::TreePop();
        }
    }
    void SceneHierarchyPanal::showComponent(Entity& entity)
    {
        if (entity.hasComponent<TagComponent>()) {
            auto& tag = entity.getComponent<TagComponent>().tag;
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.c_str());
            if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
                tag = std::string(buffer);
        }
        if (entity.hasComponent<TransformComponent>()) {
            if(ImGui::TreeNodeEx((const void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
                auto& transform = entity.getComponent<TransformComponent>().transform;
                ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
                ImGui::TreePop();
            }
        }
    }
}