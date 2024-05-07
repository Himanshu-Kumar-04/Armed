#include "ArmPCH.h"
#include "renderer.h"
#include "OpenGL/openGLShader.h"

namespace Arm {

    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

    void Renderer::init()
    {
        RendererCommand::init();
    }
    void Renderer::beginScene(OrthographicCamera& camera)
    {
        m_SceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
    }
    void Renderer::endScene()
    {
    }
    void Renderer::submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniformMat4("u_ViewProjection", m_SceneData->viewProjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniformMat4("u_Transform", transform);

        vertexArray->bind();
        RendererCommand::drawIndexed(vertexArray);
    }
}