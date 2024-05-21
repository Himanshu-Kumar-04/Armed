#include "ArmPCH.h"
#include "renderer.h"
#include "OpenGL/openGLShader.h"
#include "renderer2D.h"

namespace Arm {

    Scope<Renderer::SceneData> Renderer::m_SceneData = CreateScope<Renderer::SceneData>();

    void Renderer::init()
    {
        RendererCommand::init();
        Renderer2D::init();
    }
    void Renderer::onWindowResize(uint32_t width, uint32_t height)
    {
        RendererCommand::setViewport(0, 0, width, height);
    }
    void Renderer::beginScene(Camera& camera)
    {
        m_SceneData->viewMatrix = camera.getViewMatrix();
        m_SceneData->projectionMatrix = camera.getProjectionMatrix();
        m_SceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
    }
    void Renderer::beginScene(OrthographicCamera& camera)
    {
        m_SceneData->viewMatrix = camera.getViewMatrix();
        m_SceneData->projectionMatrix = camera.getProjectionMatrix();
        m_SceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
    }
    void Renderer::endScene()
    {
    }
    void Renderer::submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const MeshType meshType)
    {
        shader->bind();
        switch (meshType) {
        case MeshType::DYNAMIC:
            std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_ViewProjection", m_SceneData->viewProjectionMatrix);
            break;
        case MeshType::STATIC:
            std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_Proj", m_SceneData->projectionMatrix);
            std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_View", m_SceneData->viewMatrix);
        }

        RendererCommand::drawIndexed(vertexArray);
    }
    //void const Renderer::DrawObject(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture2D, const glm::vec4& tintColor)
    //{
        
    //}
}