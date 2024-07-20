#include "sceneHierarchyPanal.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include "glm/gtc/type_ptr.hpp"

#include <Armed/scene/sceneSerializer.h>

namespace Arm {
    SceneHierarchyPanal::SceneHierarchyPanal(const Ref<Scene>& context)
    {
        setContext(context);
    }
    void SceneHierarchyPanal::setContext(const Ref<Scene>& context)
    {
        m_Context = context;
        m_SelectionContext = {};
    }
    void SceneHierarchyPanal::onImGuiRender(std::vector<Ref<Scene>> scenes, AssetPack& assetPack)
    {
        ImGui::Begin("Scene Explorer", 0,  ImGuiWindowFlags_MenuBar);
        
        drawSceneExplorerMenu(scenes, assetPack);
        
        auto view = m_Context->m_Registry.view<TagComponent>();
        for (auto entityHandle : view) {
            Entity entity(entityHandle, m_Context.get());
            
            auto& tag = entity.getComponent<TagComponent>().tag;
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0);
            flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
            bool opened = ImGui::TreeNodeEx((const void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

            if (ImGui::IsItemClicked())
                m_SelectionContext = entity;

            bool entityDeleted = false;
            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Destroy Entity"))
                    entityDeleted = true;
                ImGui::EndPopup();
            }

            if (opened) {
                ImGui::TreePop();
            }

            if (entityDeleted) {
                if(m_SelectionContext == entity)
                    m_SelectionContext = {};
                m_Context->destroyEntity(entity);
            }
        }

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectionContext = {};

        if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight)) {
            if (ImGui::MenuItem("Create Empty Entity"))
                m_Context->createEntity();
            ImGui::EndPopup();
        }
        ImGui::End();

        ImGui::Begin("Properties");
        if (m_SelectionContext) {
            drawComponents(m_SelectionContext);
        }

        ImGui::End();
    }
    void SceneHierarchyPanal::drawSceneExplorerMenu(std::vector<Ref<Scene>> scenes, AssetPack& assetPack)
    {
        if (ImGui::BeginMenuBar()) {
            
            //Scene Name
            auto& tag = m_Context->getSceneName();
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.c_str());
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
            if (ImGui::InputText("##Scene", buffer, sizeof(buffer))) {
                if(assetPack.sceneMap.find(m_Context->getSceneName()) != assetPack.sceneMap.end()) {
                    std::vector<UUID> sceneData = assetPack.sceneMap[tag];
                    assetPack.sceneMap.erase(tag);
                    tag = std::string(buffer);
                    assetPack.sceneMap.insert({ tag, sceneData });
                }
                else
                    tag = std::string(buffer);
            }
            ImGui::PopItemWidth();


            // Scenes
            if (ImGui::BeginMenu("...")) {
                for (Ref<Scene> scene : scenes) {
                    if (ImGui::MenuItem(scene->getSceneName().c_str(), NULL, false)) {
                        if (scene->getSceneName() != m_Context->getSceneName()) {
                            setContext(scene);
                        }
                    }
                }
                ImGui::EndMenu();
            }
            if (ImGui::Button("Save"))
                SceneSerializer::serialize(assetPack, m_Context);
            if (ImGui::Button("Reset")) {
                m_SelectionContext = {};
                SceneSerializer::deserialize(assetPack, m_Context);
            }
            ImGui::EndMenuBar();
        }
    }

    void SceneHierarchyPanal::drawEntityNode(Entity& entity)
    {
        
    }

    static void DrawVec3Control(const std::string& label, glm::vec3& components, float resetValue = 0.0f, float columnWidth = 100.0f) {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[1];

        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());

        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f,0.0f });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("X", buttonSize))
            components.x = resetValue;
        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::DragFloat("##X", &components.x, 0.1f);
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PopStyleColor(3);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.08f, 0.67f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.06f, 0.8f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.08f, 0.67f, 0.2f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Y", buttonSize))
            components.y = resetValue;
        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::DragFloat("##Y", &components.y, 0.1f);
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PopStyleColor(3);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.2f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.1f, 0.3f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.2f, 0.8f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Z", buttonSize))
            components.z = resetValue;
        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::DragFloat("##Z", &components.z, 0.1f);
        ImGui::PopItemWidth();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);

        ImGui::Columns(1);

        ImGui::PopID();
    }

    template<typename T, typename S>
    static void DrawComponent(const std::string& tag, Entity& entity, S lembdaFunction, bool removable = true) {
        int treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_Framed;
        if (entity.hasComponent<T>()) {
            auto& component = entity.getComponent<T>();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4.0f,4.0f });
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();

            bool open = ImGui::TreeNodeEx((const void*)typeid(T).hash_code(), treeNodeFlags, tag.c_str());

            ImGui::PopStyleVar();
            bool removeComponent = false;

            if (removable) {
                ImGui::SameLine(ImGui::GetContentRegionAvail().x - lineHeight * 0.5f);

                if (ImGui::Button("-", ImVec2{ lineHeight, lineHeight }))
                    removeComponent = true;
            }

            if (open) {
                lembdaFunction(component);
                ImGui::TreePop();
            }

            if (removable && removeComponent)
                entity.removeComponent<T>();
        }
    }

    void SceneHierarchyPanal::drawComponents(Entity& entity)
    {
        if (entity.hasComponent<TagComponent>()) {
            auto& tag = entity.getComponent<TagComponent>().tag;
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.c_str());
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
                tag = std::string(buffer);
            ImGui::PopItemWidth();
        }

        ImGui::SameLine();
        if (entity.hasComponent<IDComponent>()) {
            ImGui::Text("ID : %llu", entity.getComponent<IDComponent>().ID);
        }

        DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {
            DrawVec3Control("Translation", component.translation);

            glm::vec3 rotation = glm::degrees(component.rotation);
            DrawVec3Control("Rotation", rotation);
            component.rotation = glm::radians(rotation);

            DrawVec3Control("Scale", component.scale);
            }, false);

        DrawComponent<CameraComponent>("Camera", entity, [](auto& component) {
            auto& camera = component.camera;
            ImGui::Checkbox("Primary", &component.isPrimary);

            const char* projectionTypeString[] = { "Orthographic", "Perspective" };
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
                ImGui::Checkbox("Fixed AspectRatio", &component.hasFixedAspectRatio);

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
            });

        DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component) {
            ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
            });

        DrawComponent<MeshComponent>("Mesh Renderer", entity, [](auto& component) {
            });

        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComponent");

        if (ImGui::BeginPopup("AddComponent")) {

            displayAddComponentEntry<CameraComponent>("Camera");
            displayAddComponentEntry<SpriteRendererComponent>("Sprite");
            displayAddComponentEntry<MeshComponent>("Mesh");

            ImGui::EndPopup();
        }
    }

    template<typename T>
    void SceneHierarchyPanal::displayAddComponentEntry(const std::string& tag)
    {
        if (!m_SelectionContext.hasComponent<T>())
            if (ImGui::MenuItem(tag.c_str())) {
                m_SelectionContext.addComponent<T>();
                ImGui::CloseCurrentPopup();
            }


    }
}