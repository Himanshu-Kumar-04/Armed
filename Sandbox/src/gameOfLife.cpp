#include "gameOfLife.h"
#include "imgui/imgui.h"
#include<random>

GameOfLife::GameOfLife()
    :Layer("GameOfLife"), m_Camera(1280.0f / 720.0f)
{
}

void GameOfLife::onAttach()
{
    m_Camera.setZoomLevel(4.0f);
    std::memset(cells, 0, width * height);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for (int32_t i = 0; i < width; i++) {
        for (int32_t j = 0; j < height; j++) {
            state[i][j] = dis(rd);
        }
    }
}

void GameOfLife::onDetach()
{
}

void GameOfLife::onUpdate(Arm::Timestep ts)
{
    ARM_PROFILE_FUNCTION();


    {
        ARM_PROFILE_SCOPE("CameraController.onUpdate");
        m_Camera.onUpdate(ts);
    }

    Arm::Renderer2D::resetStats();
    {
        ARM_PROFILE_SCOPE("Renderer:prep");
        Arm::RendererCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        Arm::RendererCommand::clearColor();
    }

    {
        ARM_PROFILE_SCOPE("Renderer:draw");
        Arm::Renderer2D::beginScene(m_Camera);
        for (int32_t i = 0; i < width; i++) {
            for (int32_t j = 0; j < height; j++) {
                cells[i][j] = state[i][j];
            }
        }

        for (int32_t i = 1; i < width - 1; i++) {
            for (int32_t j = 1; j < height - 1; j++) {
                uint8_t neighbours = 
                    cells[i-1][j+1] + cells[i+0][j+1] + cells[i+1][j+1] +
                    cells[i-1][j+0] +           0     + cells[i+1][j+0] +
                    cells[i-1][j-1] + cells[i+0][j-1] + cells[i+1][j-1];
                
                if (cells[i][j] == 1)
                    state[i][j] = neighbours == 2 || neighbours == 3;
                else
                    state[i][j] = neighbours == 3;
                //draw
                if(cells[i][j] == 1)
                    Arm::Renderer2D::drawQuad({ (float)i*0.1,(float)j*0.1,0.0f }, { 0.1f,0.1f }, glm::vec4(1.0f));
            }
        }

        Arm::Renderer2D::endScene();
    }

}

void GameOfLife::onImGuiRender()
{
    ImGui::Begin("Armed");

    auto stats = Arm::Renderer2D::getStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls:     %d", stats.drawCalls);
    ImGui::Text("Quad:           %d", stats.quadCount);
    ImGui::Text("Vertices:       %d", stats.getTotalVertexCount());
    ImGui::Text("Indices:        %d", stats.getTotalIndexCount());
    if (ImGui::Button("Exit")) Arm::Application::get().close();
    ImGui::End();
}

void GameOfLife::onEvent(Arm::Event& e)
{
    m_Camera.onEvent(e);
}

