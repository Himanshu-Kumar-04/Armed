#include "ArmPCH.h"
#include <Armed/renderer/rendererCommand.h>
#include "renderer2D.h"
#include<glm/gtc/matrix_transform.hpp>


namespace Arm {

    struct renderer2DData {
        Ref<VertexArray> quadVertexArray;
        Ref<Shader> quadShader;
        Ref<Shader> textureShader;
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

        s_Data->quadVertexArray = VertexArray::Create();
        Ref<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->setLayout({
                {ShaderDataType::Float3 , "position"},
                {ShaderDataType::Float2 , "texCoord"},
            });
        s_Data->quadVertexArray->addVertexBuffer(squareVB);
        uint32_t squareIndices[6] = { 0,1,2 ,2 ,3, 0 };
        Ref<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        s_Data->quadVertexArray->setIndexBuffer(squareIB);

        s_Data->quadShader = Shader::create("assets/shader/flat_color.glsl");
        s_Data->textureShader = Shader::create("assets/shader/texture.glsl");
        s_Data->textureShader->bind();
        s_Data->textureShader->setInt("u_Texture", 0);
    }

    void Renderer2D::shutdown()
    {
        delete s_Data;
    }

    void Renderer2D::beginScene(const OrthographicCamera& camera)
    {
        s_Data->quadShader->bind();
        s_Data->quadShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());

        s_Data->textureShader->bind();
        s_Data->textureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
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
        s_Data->quadShader->bind();
        s_Data->quadShader->setFloat4("u_Color", color);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data->quadShader->setMat4("u_Transform", transform);
        
        s_Data->quadVertexArray->bind();
        RendererCommand::drawIndexed(s_Data->quadVertexArray);
    }
    void const Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture)
    {
        drawQuad({ position.x, position.y, 0.0f }, size, texture);
    }
    void const Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture)
    {
        s_Data->textureShader->bind();
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data->textureShader->setMat4("u_Transform", transform);

        texture->bind();

        s_Data->quadVertexArray->bind();
        RendererCommand::drawIndexed(s_Data->quadVertexArray);
    }
}