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
        if (entity.hasComponent<TransformComponent>())
            if (ImGui::TreeNodeEx((const void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
                auto& transform = entity.getComponent<TransformComponent>().transform;
                ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
                ImGui::TreePop();
            }
        if (entity.hasComponent<CameraComponent>())
            if (ImGui::TreeNodeEx((const void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {
                auto& cameraComponent = entity.getComponent<CameraComponent>();
                auto& camera = entity.getComponent<CameraComponent>().camera;

                ImGui::Checkbox("Primary", &cameraComponent.isPrimary);

                const char* projectionTypeString[] = { "Orthographic", "Projection" };
                const char* currentProjectionTypeString = projectionTypeString[(int)camera.getProjectionType()];
                if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
                    for (uint8_t i = 0; i < 2; i++) {
                        bool isSelected = currentProjectionTypeString == projectionTypeString[i];
                        if (ImGui::Selectable(projectionTypeString[i], isSelected)) {
                            currentProjectionTypeString = projectionTypeString[i];
                            camera.setProjectionType((Camera::ProjectionType)i);
                        }

                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }
                        ImGui::EndCombo();
                }

                if (camera.getProjectionType() == SceneCamera::ProjectionType::orthographic) {
                    ImGui::Checkbox("Fixed AspectRatio", &cameraComponent.hasFixedAspectRatio);
                    float size = camera.getOrthographicSize();
                    if (ImGui::DragFloat("Size", &size))
                        camera.setOrthographicSize(size);
                }
                else if (camera.getProjectionType() == SceneCamera::ProjectionType::perspective) {
                    float verticalFOV = camera.getPerspectiveVerticalFOV();
                    if (ImGui::DragFloat("Vertical FOV", &verticalFOV))
                        camera.setPerspectiveVerticalFOV(verticalFOV);
                }

                float nearClip = camera.getNearClip();
                if (ImGui::DragFloat("Near", &nearClip))
                    camera.setNearClip(nearClip);

                float farClip = camera.getFarClip();
                if (ImGui::DragFloat("Far", &farClip))
                    camera.setFarClip(farClip);

                ImGui::TreePop();
            }
    }
}