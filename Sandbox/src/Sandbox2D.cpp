#include "sandbox2D.h"
#include "imgui/imgui.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    :Layer("SandBox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::onAttach()
{
    m_SpritesSheet = Arm::Texture2D::Create("assets/game/assets/RPGpack_sheet_2X.png");
    m_CameraController.setZoomLevel(5.0f);
}

void Sandbox2D::onDetach()
{
}

void Sandbox2D::onUpdate(Arm::Timestep ts)
{
    ARM_PROFILE_FUNCTION();


    {
        ARM_PROFILE_SCOPE("CameraController.onUpdate");
        m_CameraController.onUpdate(ts);
    }

    Arm::Renderer2D::resetStats();
    {
        ARM_PROFILE_SCOPE("Renderer:prep");
        Arm::RendererCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        Arm::RendererCommand::clearColor();
    }

    {
        ARM_PROFILE_SCOPE("Renderer:draw");
        static float rotation = 0.0f;
        rotation += ts * 9.0f;
        Arm::Renderer2D::beginScene(m_CameraController.getCamera());
        Arm::Renderer2D::drawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f,1.0f }, { 0.2f,0.6f,0.4f,1.0f });
        Arm::Renderer2D::endScene();
    }

}

void Sandbox2D::onImGuiRender()
{
    ImGui::Begin("Settings");
    auto stats = Arm::Renderer2D::getStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls:     %d", stats.drawCalls);
    ImGui::Text("Quad:           %d", stats.quadCount);
    ImGui::Text("Vertices:       %d", stats.getTotalVertexCount());
    ImGui::Text("Indices:        %d", stats.getTotalIndexCount());
    if (ImGui::Button("Exit")) Arm::Application::get().close();
    ImGui::End();
}

void Sandbox2D::onEvent(Arm::Event& e)
{
    m_CameraController.onEvent(e);
}

