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
    void Renderer::beginScene(const Camera& camera, const glm::mat4& transform)
    {
        m_SceneData->projectionMatrix = camera.getProjection() * glm::inverse(transform);
    }
    void Renderer::endScene()
    {
    }
    void Renderer::submit(const Ref<Shader>& shader, const Ref<VertexBuffer>& vertexBuffer)
    {
        shader->bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_ViewProjection", m_SceneData->viewProjectionMatrix);
        RendererCommand::drawIndexed(vertexBuffer);
    }
    //void const Renderer::DrawObject(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture2D, const glm::vec4& tintColor)
    //{
        
    //}
}