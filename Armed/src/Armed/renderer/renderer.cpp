#include "ArmPCH.h"
#include "renderer.h"
#include "OpenGL/openGLShader.h"
#include "renderer2D.h"

namespace Arm {

    struct RendererData {
        static const uint32_t maxTextureSlots = 32; // TODO: RENDER_CAPS
        
        Ref<Shader> shader;
        Ref<Texture2D> whiteTexture;
    };

    static RendererData s_Data;

    Scope<Renderer::SceneData> Renderer::m_SceneData = CreateScope<Renderer::SceneData>();

    void Renderer::init()
    {
        s_Data.whiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

        int32_t sampler[s_Data.maxTextureSlots];
        for (uint32_t i = 0; i < s_Data.maxTextureSlots; i++) {
            sampler[i] = i;
        }
        s_Data.shader = Shader::create("assets/shader/texture.glsl");
        s_Data.shader->bind();
        s_Data.shader->setIntArray("u_Textures", sampler, s_Data.maxTextureSlots);
    }

    void Renderer::onWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::setViewport(0, 0, width, height);
    }

    void Renderer::beginScene(const Camera& camera, const glm::mat4& transform)
    {
        s_Data.shader->setMat4("u_ViewProjection", camera.getProjection() * glm::inverse(transform));
    }
    void Renderer::endScene()
    {
    }

    void Renderer::submit(const glm::mat4& transform, Mesh& mesh)
    {
        mesh.m_VertexBuffer->setData(mesh.updateTransform(transform).data(), mesh.vertices.size() * sizeof(VertexData));
        s_Data.whiteTexture->bind(0);
        for (uint8_t i = 1; i < mesh.textures.size(); i++)
            mesh.textures[i]->bind(i);
        RenderCommand::drawIndexed(mesh.m_VertexBuffer, mesh.indices.size());
    }
}