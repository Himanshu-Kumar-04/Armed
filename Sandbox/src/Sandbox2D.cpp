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
    m_Texture1 = Arm::Texture2D::Create("assets/texture/Armed.jpeg");
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

    {
        ARM_PROFILE_SCOPE("Renderer:prep");
        Arm::RendererCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        Arm::RendererCommand::clearColor();
    }

    {
        ARM_PROFILE_SCOPE("Renderer:beginScene");
        Arm::Renderer2D::beginScene(m_CameraController.getCamera());
    }

    {
        ARM_PROFILE_SCOPE("Renderer:draw");
        Arm::Renderer2D::drawQuad({ -1.0f,0.0f }, { 0.4f,0.4f }, { 0.7f,0.2,0.3f,1.0f });
        Arm::Renderer2D::drawQuad({ 0.5f,-0.5f }, { 0.2f,0.6f }, { 0.2f,0.3,0.6f,1.0f });
        Arm::Renderer2D::drawQuad({ 0.2f,0.2f, -0.1f }, { 10.0f,10.0f }, m_Texture1);
        Arm::Renderer2D::endScene();
    }

}

void Sandbox2D::onImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::Text("s");
    ImGui::End();
}

void Sandbox2D::onEvent(Arm::Event& e)
{
    m_CameraController.onEvent(e);
}

