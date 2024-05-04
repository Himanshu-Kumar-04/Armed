#include "ArmPCH.h"
#include "renderer.h"

namespace Arm {

    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
    void Renderer::beginScene(OrthographicCamera& camera)
    {
        m_SceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
    }
    void Renderer::endScene()
    {
    }
    void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
    {
        shader->bind();
        shader->setUniformMat4("u_ViewProjection", m_SceneData->viewProjectionMatrix);
        vertexArray->bind();
        RendererCommand::drawIndexed(vertexArray);
    }
}