#include "EditorLayer.h"
#include <vector>
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Armed/scene/sceneSerializer.h>

namespace Arm {
    EditorLayer::EditorLayer()
        :Layer("EditorLayer")
    {
    }

    void EditorLayer::onAttach()
    {
        FrameBufferProperties fbProps;
        fbProps.width = Application::get().getWindow().getWidth();
        fbProps.height = Application::get().getWindow().getHeight();

        m_FrameBuffer = FrameBuffer::create(fbProps);

        m_Scene = CreateRef<Scene>("Black&White");
        m_Scenes.push_back(m_Scene);
        m_Scenes.push_back(CreateRef<Scene>("Blank"));
        //m_Scene->createEntity("cube").addComponent<MeshComponent>();

        float n = 5.0f;
        int num = 0;
        for(float i=0.0f; i<n; i++)
            for (float j = 0.0f; j < n; j++) {
                num++;
                float color = (float)(num % 2);
                Entity square = m_Scene->createEntity("square");
                square.addComponent<SpriteRendererComponent>(glm::vec4{color, color, color ,1.0});
                glm::vec3& translation = square.getComponent<TransformComponent>().translation;
                translation.x = i;
                translation.y = j;
            }
        m_CameraEntity = m_Scene->createEntity("camera entity");

        m_CameraEntity.addComponent<CameraComponent>();
        class CameraController : public ScriptableEntity {
        public:
            virtual void onCreate() override {
                auto& translation = getComponent<TransformComponent>().translation;
                translation.z = 60.0f;
                translation.x = 1.0f;
                translation.y = 1.0f;
            }
            virtual void onDestroy() override {
            }
            virtual void onUpdate(Timestep ts) override {
                
                auto& translation = getComponent<TransformComponent>().translation;
                float speed = 0.1f * translation.z;
                if (Input::isKeyPressed(Key::A)) translation.x -= speed * ts;
                if (Input::isKeyPressed(Key::D)) translation.x += speed * ts;
                if (Input::isKeyPressed(Key::W)) translation.y += speed * ts;
                if (Input::isKeyPressed(Key::S)) translation.y -= speed * ts;
            }
        };

        m_CameraEntity.addComponent<NativeScriptComponent>().bind<CameraController>();
        m_SceneHierarchyPanal.setContext(m_Scenes[0]);
    }

    void EditorLayer::onDetach()
    {
    }

    void EditorLayer::onUpdate(Timestep ts)
    {
        m_Scene = m_SceneHierarchyPanal.getContext();
        m_Scene->onViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

        if (FrameBufferProperties props = m_FrameBuffer->getProperties();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (props.width != m_ViewportSize.x || props.height != m_ViewportSize.y))
        {
            m_FrameBuffer->resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        Renderer2D::resetStats();
        m_FrameBuffer->bind();

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
            if (ImGui::BeginMenu("File")) {
                //ImGui::Separator();
                if (ImGui::MenuItem("Close", NULL, false, p != NULL))
                {
                    *p = false;
                }
                if (ImGui::MenuItem("Save", NULL, false, p != NULL))
                {
                    TextSerializer::serializeAssets(m_AssetFilePath, m_AssetPack);
                }
                if (ImGui::MenuItem("Open", NULL, false, p != NULL))
                {
                    TextSerializer::deserializeAssets(m_AssetFilePath, m_AssetPack);
                    m_Scenes.clear();
                    for (auto sc : m_AssetPack.sceneMap) {
                        m_Scene = CreateRef<Scene>(sc.first);
                        SceneSerializer::deserialize(m_AssetPack, m_Scene);
                        m_Scenes.push_back(m_Scene);
                    }
                    m_SceneHierarchyPanal.setContext(m_Scene);
                }
                //if (ImGui::MenuItem("Save As", "Ctrl + S"));
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        m_SceneHierarchyPanal.onImGuiRender(m_Scenes, m_AssetPack);

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
        
        if (!ImGui::IsWindowFocused())
            m_Scene->setSceneState(Scene::SceneState::paused);
        else
            m_Scene->setSceneState(Scene::SceneState::running);

        Application::get().getImGuiLayer()->BlockEvents(!ImGui::IsWindowFocused());

        ImVec2 viewportPanalSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanalSize.x, viewportPanalSize.y };

        uintptr_t textureId = m_FrameBuffer->getColorAttachmentRendererID();
        ImGui::Image((void*)textureId, {m_ViewportSize.x,m_ViewportSize.y}, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
        
        ImGui::PopStyleVar();

        ImGui::End();
    }

    void EditorLayer::onEvent(Event& e)
    {
    }

}