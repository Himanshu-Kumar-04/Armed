#include "EditorLayer.h"
#include "imgui/imgui.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

namespace Arm {
    EditorLayer::EditorLayer()
        :Layer("EditorLayer"), m_Camera(1280.0f / 720.0f)
    {
    }

    void EditorLayer::onAttach()
    {
        m_SpritesSheet = Texture2D::Create("assets/game/assets/RPGpack_sheet_2X.png");
        m_Camera.setZoomLevel(4.0f);

        FrameBufferProperties fbProps;
        fbProps.width = 1280;
        fbProps.height = 720;

        m_FrameBuffer = FrameBuffer::create(fbProps);
        
        m_Scene = CreateRef<Scene>();
        Entity square = m_Scene->createEntity();
        square.addComponent<SpriteRendererComponent>(glm::vec4{0.0f,1.0f,0.0f,1.0});

    }

    void EditorLayer::onDetach()
    {
    }

    void EditorLayer::onUpdate(Timestep ts)
    {
        t = ts;
        if (m_ViewportFocused)
            m_Camera.onUpdate(ts);

        Renderer2D::resetStats();
        m_FrameBuffer->bind();
        RendererCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        RendererCommand::clearColor();

        Arm::Renderer2D::beginScene(m_Camera);

        m_Scene->onUpdate(ts);

        Arm::Renderer2D::endScene();
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
            if (ImGui::BeginMenu("Options")) {
                //ImGui::Separator();
                if (ImGui::MenuItem("Close", NULL, false, p != NULL))
                    *p = false;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        ImGui::Begin("Stats");

        auto stats = Renderer2D::getStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("frame rate:     %f", 1/t);
        ImGui::Text("Draw Calls:     %d", stats.drawCalls);
        ImGui::Text("Quad:           %d", stats.quadCount);
        ImGui::Text("Vertices:       %d", stats.getTotalVertexCount());
        ImGui::Text("Indices:        %d", stats.getTotalIndexCount());
        if (ImGui::Button("Exit")) Application::get().close();
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport");
        
        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::get().getImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

        ImVec2 viewportPanalSize = ImGui::GetContentRegionAvail();
        if (m_ViewportSize != *(glm::vec2*)&viewportPanalSize) {
            m_FrameBuffer->resize((uint32_t)viewportPanalSize.x, (uint32_t)viewportPanalSize.y);
            m_ViewportSize = { viewportPanalSize.x, viewportPanalSize.y };

            m_Camera.onResize(viewportPanalSize.x, viewportPanalSize.y);
        }

        uint32_t textureId = m_FrameBuffer->getColorAttachmentRendererID();
        ImGui::Image((void*)textureId, { m_ViewportSize.x,m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
        
        ImGui::PopStyleVar();

        ImGui::End();
    }

    void EditorLayer::onEvent(Event& e)
    {
        m_Camera.onEvent(e);
    }

}