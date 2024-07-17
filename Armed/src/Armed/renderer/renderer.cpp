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

    static RendererData s_R3DData;

    Scope<Renderer::SceneData> Renderer::m_SceneData = CreateScope<Renderer::SceneData>();

    void Renderer::init()
    {
        RenderCommand::init();
        Renderer2D::init();

        s_R3DData.whiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_R3DData.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

        int32_t sampler[s_R3DData.maxTextureSlots];
        for (uint32_t i = 0; i < s_R3DData.maxTextureSlots; i++) {
            sampler[i] = i;
        }
        s_R3DData.shader = Shader::create("assets/shader/texture.glsl");
        s_R3DData.shader->bind();
        s_R3DData.shader->setIntArray("u_Textures", sampler, s_R3DData.maxTextureSlots);
    }

    void Renderer::onWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::setViewport(0, 0, width, height);
    }

    void Renderer::beginScene(const Camera& camera, const glm::mat4& transform)
    {
        s_R3DData.shader->setMat4("u_ViewProjection", camera.getProjection() * glm::inverse(transform));
    }
    void Renderer::endScene()
    {
    }

    void Renderer::submit(const glm::mat4& transform, Mesh& mesh)
    {
        s_R3DData.whiteTexture->bind(0);
        mesh.draw(transform);
    }
}