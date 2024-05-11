#include "ArmPCH.h"
#include <Armed/renderer/rendererCommand.h>
#include "renderer2D.h"
#include<glm/gtc/matrix_transform.hpp>



namespace Arm {

    struct renderer2DData {
        Ref<VertexArray> vertexArray;
        Ref<Shader> shader;
        Ref<Texture2D> whiteTexture;
    };

    static renderer2DData* s_Data;

    void Renderer2D::init()
    {
        float squareVertices[4 * 5] = {
            -0.5f ,-0.5f, 0.0f, 0.0f, 0.0f,
             0.5f ,-0.5f, 0.0f, 1.0f, 0.0f,
             0.5f , 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f , 0.5f, 0.0f, 0.0f, 1.0f
        };

        s_Data = new renderer2DData();

        s_Data->vertexArray = VertexArray::Create();
        Ref<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->setLayout({
                {ShaderDataType::Float3 , "position"},
                {ShaderDataType::Float2 , "texCoord"},
            });
        s_Data->vertexArray->addVertexBuffer(squareVB);
        uint32_t squareIndices[6] = { 0,1,2 ,2 ,3, 0 };
        Ref<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        s_Data->vertexArray->setIndexBuffer(squareIB);

        s_Data->whiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data->whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

        s_Data->shader = Shader::create("assets/shader/texture.glsl");
        s_Data->shader->bind();
        s_Data->shader->setInt("u_Texture", 0);
    }

    void Renderer2D::shutdown()
    {
        delete s_Data;
    }

    void Renderer2D::beginScene(const OrthographicCamera& camera)
    {
        s_Data->shader->bind();
        s_Data->shader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
    }

    void Renderer2D::endScene()
    {
    }

    void const Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        drawQuad({ position.x, position.y, 0.0f }, size, color);
    }

    void const Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        s_Data->shader->bind();
        s_Data->shader->setFloat4("u_Color", color);
        s_Data->whiteTexture->bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data->shader->setMat4("u_Transform", transform);
        
        s_Data->vertexArray->bind();
        RendererCommand::drawIndexed(s_Data->vertexArray);
    }
    void const Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& tintColor)
    {
        drawQuad({ position.x, position.y, 0.0f }, size, texture);
    }
    void const Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& tintColor)
    {
        s_Data->shader->bind();
        s_Data->shader->setFloat4("u_Color", tintColor);
        texture->bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data->shader->setMat4("u_Transform", transform);

        s_Data->vertexArray->bind();
        RendererCommand::drawIndexed(s_Data->vertexArray);
    }
}