#include "sandbox2D.h"
#include "imgui/imgui.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    :Layer("SandBox2D"), m_Camera(1280.0f / 720.0f)
{
}

void Sandbox2D::onAttach()
{
    m_SpritesSheet = Arm::Texture2D::Create("assets/game/assets/RPGpack_sheet_2X.png");
    m_Camera.setZoomLevel(4.0f);

    Arm::FrameBufferProperties fbProps;
    fbProps.width = 1280;
    fbProps.height = 720;

    m_FrameBuffer = Arm::FrameBuffer::create(fbProps);
}

void Sandbox2D::onDetach()
{
}

void Sandbox2D::onUpdate(Arm::Timestep ts)
{
    ARM_PROFILE_FUNCTION();


    {
        ARM_PROFILE_SCOPE("CameraController.onUpdate");
        m_Camera.onUpdate(ts);
    }

    Arm::Renderer2D::resetStats();
    {
        ARM_PROFILE_SCOPE("Renderer:prep");
        m_FrameBuffer->bind();
        Arm::RendererCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        Arm::RendererCommand::clearColor();
    }

    {
        ARM_PROFILE_SCOPE("Renderer:draw");
        static float rotation = 0.0f;
        rotation += ts * 9.0f;
        Arm::Renderer2D::beginScene(m_Camera);
        Arm::Renderer2D::drawRotatedQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f,1.0f }, rotation, {0.5f,0.3f,0.5f,1.0f});
        Arm::Renderer2D::endScene();
        m_FrameBuffer->unbind();
    }

}

void Sandbox2D::onImGuiRender()
{
#if 1
    static bool s = false;
    bool *p = &s;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", p, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    else
    {
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
            if (ImGui::MenuItem("Flag: NoDockingSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
            if (ImGui::MenuItem("Flag: NoUndocking", "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            if (ImGui::MenuItem("Close", NULL, false, p != NULL))
                *p = false;
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::Begin("Armed");

    auto stats = Arm::Renderer2D::getStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls:     %d", stats.drawCalls);
    ImGui::Text("Quad:           %d", stats.quadCount);
    ImGui::Text("Vertices:       %d", stats.getTotalVertexCount());
    ImGui::Text("Indices:        %d", stats.getTotalIndexCount());
    if (ImGui::Button("Exit")) Arm::Application::get().close();
    uint32_t textureId = m_FrameBuffer->getColorAttachmentRendererID();
    ImGui::Image((void*)textureId, { 1280,720 });
    ImGui::End();

    ImGui::End();
#endif
}

void Sandbox2D::onEvent(Arm::Event& e)
{
    m_Camera.onEvent(e);
}

