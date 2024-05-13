#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    :Layer("SandBox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::onAttach()
{
    m_Texture1 = Arm::Texture2D::Create("assets/texture/grass_tile.png");
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

        Arm::Renderer2D::drawRotatedQuad({ -2.0f,0.1f }, { 0.8f,0.8f }, rotation, { 0.7f,0.2f,0.3f,1.0f });
        Arm::Renderer2D::drawRotatedQuad({ -1.0f,0.1f }, { 0.8f,0.8f }, -rotation ,{ 0.7f,0.2f,0.3f,1.0f });
        Arm::Renderer2D::drawQuad({ 0.5f,-0.5f }, { 0.5f,0.75f }, { 0.2f,0.3f,0.6f,1.0f });
        Arm::Renderer2D::drawQuad({ 0.0f,0.0f, -0.1f }, { 20.0f,20.0f }, m_Texture1, 10.0f);
        
        for (float y = -5.0f; y <= 5.0f; y += 0.5f) {
            for (float x = -5.0f; x <= 5.0f; x += 0.5f) {
                glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f };
                Arm::Renderer2D::drawQuad({ x,y }, { 0.45f,0.45f }, color);
            }
        }
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
    ImGui::End();
}

void Sandbox2D::onEvent(Arm::Event& e)
{
    m_CameraController.onEvent(e);
}

