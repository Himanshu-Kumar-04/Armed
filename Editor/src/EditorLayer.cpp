#include "EditorLayer.h"
#include <vector>
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Armed/scene/sceneSerializer.h>
#include <Armed/utils/utils.h>

#include <ImGuizmo.h>

namespace Arm {
    EditorLayer::EditorLayer()
        :Layer("EditorLayer")
    {
    }

    void EditorLayer::onAttach()
    {
        FrameBufferProperties fbProps;
        fbProps.width = Application::Get().getWindow().getWidth();
        fbProps.height = Application::Get().getWindow().getHeight();

        m_FrameBuffer = FrameBuffer::create(fbProps);

        m_ActiveScene = m_Scenes->createNewScene();

        m_SceneHierarchyPanal.setContext(m_ActiveScene);
        SceneSerializer::serialize(m_AssetPack, m_ActiveScene);
    }

    void EditorLayer::onDetach()
    {
    }

    void EditorLayer::onUpdate(Timestep ts)
    {
        m_ActiveScene->onViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

        if (FrameBufferProperties props = m_FrameBuffer->getProperties();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (props.width != m_ViewportSize.x || props.height != m_ViewportSize.y))
        {
            m_FrameBuffer->resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        Renderer2D::resetStats();
        m_FrameBuffer->bind();

        m_ActiveScene->onUpdate(ts);

        m_FrameBuffer->unbind();

    }

    void EditorLayer::onImGuiRender()
    {
        static bool s = false;
        bool* p = &s;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", p, window_flags);

        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New", "Ctrl+N"))
                    newFile();
                ImGui::Separator();
                if (ImGui::MenuItem("Open", "Ctrl+O"))
                    openFile();
                ImGui::Separator();
                if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
                    saveFile();
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        m_SceneHierarchyPanal.onImGuiRender(m_Scenes, m_AssetPack, m_SelectedEntity);

        ImGui::Begin("Stats");

        auto stats = Renderer2D::getStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls:     %d", stats.drawCalls);
        ImGui::Text("Quad:           %d", stats.quadCount);
        ImGui::Text("Vertices:       %d", stats.getTotalVertexCount());
        ImGui::Text("Indices:        %d", stats.getTotalIndexCount());
        if (ImGui::Button("Exit")) Application::Get().close();

        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("Viewport");


        ImVec2 viewportPanalSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanalSize.x, viewportPanalSize.y };

        uintptr_t textureId = m_FrameBuffer->getColorAttachmentRendererID();
        ImGui::Image(reinterpret_cast<void*>(textureId),{ m_ViewportSize.x,m_ViewportSize.y },ImVec2(0, 1),ImVec2(1, 0));

        ///ImGuizmo 
        if (m_SelectedEntity && m_GuizmoType != -1) {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

            //Camera
            Entity cameraEntity               = m_ActiveScene->getPrimaryCameraEntity();
            const glm::mat4& cameraProjection = cameraEntity.getComponent<CameraComponent>().camera.getProjection();
            glm::mat4 cameraView              = glm::inverse(cameraEntity.getComponent<TransformComponent>().getTransform());
            //Entity
            TransformComponent& tc            = m_SelectedEntity.getComponent<TransformComponent>();
            glm::mat4 transform               = tc.getTransform();

            // Snapping
            bool snap = Input::isKeyPressed(Key::LeftControl);
            float snapValue = 0.5f; // Snap to 0.5m for translation/scale // Snap to 45 degrees for rotation
            if (m_GuizmoType == ImGuizmo::OPERATION::ROTATE)
                snapValue = 45.0f;

            float snapValues[3] = { snapValue, snapValue, snapValue };

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                (ImGuizmo::OPERATION)m_GuizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
                nullptr, snap ? snapValues : nullptr);

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 translation, rotation, scale;
                Math::DecomposeTransform(transform, translation, rotation, scale);
                const glm::vec3 deltaRotation = rotation - tc.rotation;

                if (m_GuizmoType == ImGuizmo::TRANSLATE)
                    tc.translation = translation;
                else if (m_GuizmoType == ImGuizmo::ROTATE)
                    tc.rotation += deltaRotation;
                else if (m_GuizmoType == ImGuizmo::SCALE)
                    tc.scale = scale;
            }
        }

        ImGui::End();

        ImGui::PopStyleVar();

        ImGui::End();
    }
    void EditorLayer::onEvent(Event& e)
    {
        EventDispatcher dispatcher(e);

        dispatcher.dispatch<KeyPressedEvent>(ARM_BIND_EVENT_FN(EditorLayer::onKeyPressed));
    }
    bool EditorLayer::onKeyPressed(KeyPressedEvent& e)
    {
        if (e.IsRepeat())
            return false;

        bool control = Input::isKeyPressed(Key::LeftControl) || Input::isKeyPressed(Key::RightControl);
        bool shift = Input::isKeyPressed(Key::LeftShift) || Input::isKeyPressed(Key::RightShift);

        switch (e.getKeyCode())
        {
        case Key::N: if (control) newFile(); break;
        case Key::O: if (control) openFile(); break;
        case Key::S: if (control && shift) saveFile(); break;

        case Key::Q: m_GuizmoType = -1; break;
        case Key::W: m_GuizmoType = ImGuizmo::OPERATION::TRANSLATE; break;
        case Key::E: m_GuizmoType = ImGuizmo::OPERATION::ROTATE;    break;
        case Key::R: m_GuizmoType = ImGuizmo::OPERATION::SCALE;     break;

        default:
            break;
        }
        return true;
    }
    void EditorLayer::newFile() {
        m_Scenes->clearScenes();
        m_ActiveScene = m_Scenes->createNewScene();
        m_SceneHierarchyPanal.setContext(m_ActiveScene);
    }
    void EditorLayer::openFile() {
        std::string filepath = FileDialog::OpenFile("Armed Asset (*.armed)\0*.armed\0");
        if (!filepath.empty()) {
            TextSerializer::deserializeAssets(filepath, m_AssetPack);
            m_Scenes->clearScenes();
            for (auto sc : m_AssetPack.sceneMap) {
                m_Scenes->createNewScene(sc.first);
                SceneSerializer::deserialize(m_AssetPack, m_Scenes->getScene(sc.first));
            }
            m_ActiveScene = m_Scenes->getActiveScene();
            m_SceneHierarchyPanal.setContext(m_ActiveScene);
        }
    }
    void EditorLayer::saveFile() {
        for (auto& scene : m_Scenes->get())
            SceneSerializer::serialize(m_AssetPack, scene.second);

        std::string filepath = FileDialog::SaveFile("Armed Asset (*.armed)\0*.armed\0");
        if (!filepath.empty())
            TextSerializer::serializeAssets(filepath, m_AssetPack);
    }
}