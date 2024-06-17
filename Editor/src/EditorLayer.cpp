#include "EditorLayer.h"
#include "imgui/imgui.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

namespace Arm {
    EditorLayer::EditorLayer()
        :Layer("EditorLayer")
    {
    }

    void EditorLayer::onAttach()
    {
        m_SpritesSheet = Texture2D::Create("assets/game/assets/RPGpack_sheet_2X.png");

        FrameBufferProperties fbProps;
        fbProps.width = 1280;
        fbProps.height = 720;

        m_FrameBuffer = FrameBuffer::create(fbProps);

        m_Scene = CreateRef<Scene>(SceneType::__2D__);
        Entity square = m_Scene->createEntity("green square");
        m_CameraEntity = m_Scene->createEntity("camera entity");
        m_CameraEntity.addComponent<CameraComponent>();
        square.addComponent<SpriteRendererComponent>(glm::vec4{0.0f,1.0f,0.0f,1.0});

        class CameraController : public ScriptableEntity {
        public:
            virtual void onCreate() override {
            }
            virtual void onDestroy() override {
            }
            virtual void onUpdate(Timestep ts) override {
                auto& transform = getComponent<TransformComponent>().transform;
                float speed = 5.0f;

                if (Input::isKeyPressed(Key::A)) transform[3][0] -= speed * ts;
                if (Input::isKeyPressed(Key::D)) transform[3][0] += speed * ts;
                if (Input::isKeyPressed(Key::W)) transform[3][1] += speed * ts;
                if (Input::isKeyPressed(Key::S)) transform[3][1] -= speed * ts;
            }
        };

        m_CameraEntity.addComponent<NativeScriptComponent>().bind<CameraController>();
        m_SceneHierarchyPanal.SetContext(m_Scene);
    }

    void EditorLayer::onDetach()
    {
    }

    void EditorLayer::onUpdate(Timestep ts)
    {
        //Resize
        if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (m_FrameBuffer->getProperties().width != m_ViewportSize.x || m_FrameBuffer->getProperties().height != m_ViewportSize.y)) {
            m_FrameBuffer->resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_Scene->onViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        Renderer2D::resetStats();
        m_FrameBuffer->bind();
        RendererCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        RendererCommand::clearColor();

        m_Scene->onUpdate(ts);

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

        m_SceneHierarchyPanal.onImGuiRender();

        ImGui::Begin("Stats");

        auto stats = Renderer2D::getStats();
        ImGui::Text("Renderer2D Stats:");
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
        m_ViewportSize = { viewportPanalSize.x, viewportPanalSize.y };

        uint32_t textureId = m_FrameBuffer->getColorAttachmentRendererID();
        ImGui::Image((void*)textureId, { m_ViewportSize.x,m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
        
        ImGui::PopStyleVar();

        ImGui::End();
    }

    void EditorLayer::onEvent(Event& e)
    {
    }

}