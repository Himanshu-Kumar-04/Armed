#include "ArmPCH.h"
#include <Armed/renderer/rendererCommand.h>
#include "renderer2D.h"
#include<glm/gtc/matrix_transform.hpp>



namespace Arm {
    struct QuadVertex {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoord;
        float texIndex;
    };

    struct Renderer2DData {
        const uint32_t maxQuadCount = 10000;
        const uint32_t maxVertexCount = maxQuadCount * 4;
        const uint32_t maxIndexCount = maxQuadCount * 6;
        static const uint32_t maxTextureSlots = 32; // TODO: RENDER_CAPS

        Ref<VertexArray> quadVertexArray;
        Ref<VertexBuffer> quadVertexBuffer;
        Ref<Shader> quadShader;
        Ref<Texture2D> whiteTexture;

        uint32_t quadIndexCount = 0;
        QuadVertex* quadVertexBufferBase = nullptr;
        QuadVertex* quadVertexBufferPtr = nullptr;

        std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
        uint32_t textureSlotIndex = 1;
    };

    static Renderer2DData s_Data;

    void Renderer2D::init()
    {
        ARM_PROFILE_FUNCTION();

        s_Data.quadVertexArray = VertexArray::Create();
        s_Data.quadVertexBuffer = VertexBuffer::Create(s_Data.maxVertexCount * sizeof(QuadVertex));
        s_Data.quadVertexBuffer->setLayout({
                {ShaderDataType::Float3 , "position"},
                {ShaderDataType::Float4 , "color"},
                {ShaderDataType::Float2 , "texCoord"},
                {ShaderDataType::Float , "texIndex"}
            });
        s_Data.quadVertexArray->addVertexBuffer(s_Data.quadVertexBuffer);
        s_Data.quadVertexBufferBase = new QuadVertex[s_Data.maxVertexCount];

        uint32_t* quadIndices = new uint32_t[s_Data.maxIndexCount];
        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.maxIndexCount; i += 6) {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;
            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;
            offset += 4;
        }
        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.maxIndexCount);
        s_Data.quadVertexArray->setIndexBuffer(quadIB);
        delete[] quadIndices;

        s_Data.whiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

        int32_t sampler[s_Data.maxTextureSlots];
        for (uint32_t i = 0; i < s_Data.maxTextureSlots; i++) {
            sampler[i] = i;
        }
        s_Data.quadShader = Shader::create("assets/shader/texture.glsl");
        s_Data.quadShader->bind();
        s_Data.quadShader->setIntArray("u_Textures",sampler, s_Data.maxTextureSlots);

        s_Data.textureSlots[0] = s_Data.whiteTexture;
    }
    
    void Renderer2D::shutdown()
    {
        ARM_PROFILE_FUNCTION();
    }

    //////////////////////////////////////
    // Batches
    ////////////////////////////////////// 

    void Renderer2D::beginBatch() {

    }

    void Renderer2D::flush() {
        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.quadVertexBufferPtr - (uint8_t*)s_Data.quadVertexBufferBase);
        s_Data.quadVertexBuffer->setData(s_Data.quadVertexBufferBase, dataSize);
        for (uint32_t i = 0; i < s_Data.textureSlotIndex; i++) {
            s_Data.textureSlots[i]->bind(i);
        }
        RendererCommand::drawIndexed(s_Data.quadVertexArray, s_Data.quadIndexCount);
    }

    //////////////////////////////////////
    // Scenes
    ////////////////////////////////////// 

    void Renderer2D::beginScene(const OrthographicCamera& camera)
    {
        ARM_PROFILE_FUNCTION();

        s_Data.quadShader->bind();
        s_Data.quadShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());

        s_Data.quadIndexCount = 0;
        s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferBase;

        s_Data.textureSlotIndex = 1;
        beginBatch();
    }

    void Renderer2D::endScene()
    {
        ARM_PROFILE_FUNCTION();

        flush();
    }

    //////////////////////////////////////
    // Draws
    ////////////////////////////////////// 
    
    void const Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        drawQuad({ position.x, position.y, 0.0f }, size, color);
    }
    void const Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        ARM_PROFILE_FUNCTION();
        const float texIndex = 0.0f;

        s_Data.quadVertexBufferPtr->position = position;
        s_Data.quadVertexBufferPtr->color = color;
        s_Data.quadVertexBufferPtr->texCoord = {0.0f, 0.0f};
        s_Data.quadVertexBufferPtr->texIndex = texIndex;
        s_Data.quadVertexBufferPtr++;

        s_Data.quadVertexBufferPtr->position = { position.x + size.x, position.y, 0.0f };
        s_Data.quadVertexBufferPtr->color = color;
        s_Data.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
        s_Data.quadVertexBufferPtr->texIndex = texIndex;
        s_Data.quadVertexBufferPtr++;

        s_Data.quadVertexBufferPtr->position = { position.x + size.x, position.y + size.y, 0.0f };
        s_Data.quadVertexBufferPtr->color = color;
        s_Data.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
        s_Data.quadVertexBufferPtr->texIndex = texIndex;
        s_Data.quadVertexBufferPtr++;

        s_Data.quadVertexBufferPtr->position = { position.x, position.y + size.y, 0.0f };
        s_Data.quadVertexBufferPtr->color = color;
        s_Data.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
        s_Data.quadVertexBufferPtr->texIndex = texIndex;
        s_Data.quadVertexBufferPtr++;

        s_Data.quadIndexCount += 6;

        /*s_Data.quadShader->bind();
        s_Data.quadShader->setFloat4("u_Color", color);
        s_Data.quadShader->setFloat("u_TilingFactor", 1.0f);
        s_Data.whiteTexture->bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data.quadShader->setMat4("u_Transform", transform);
        
        s_Data.quadVertexArray->bind();
        RendererCommand::drawIndexed(s_Data.quadVertexArray);*/
    }

    void const Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, const float tilingFactor, const glm::vec4& tintColor)
    {
        drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
    }
    void const Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const float tilingFactor, const glm::vec4& tintColor)
    {
        ARM_PROFILE_FUNCTION();
        glm::vec4 color = glm::vec4(1.0f);

        float textureIndex = 0.0f;

        for (uint32_t i = 1; i < s_Data.textureSlotIndex; i++) {
            if (*s_Data.textureSlots[i].get() == *texture.get()) {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0) {
            textureIndex = (float)s_Data.textureSlotIndex;
            s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
            s_Data.textureSlotIndex++;
        }

        s_Data.quadVertexBufferPtr->position = position;
        s_Data.quadVertexBufferPtr->color = color;
        s_Data.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
        s_Data.quadVertexBufferPtr->texIndex = textureIndex;
        s_Data.quadVertexBufferPtr++;

        s_Data.quadVertexBufferPtr->position = { position.x + size.x, position.y, 0.0f };
        s_Data.quadVertexBufferPtr->color = color;
        s_Data.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
        s_Data.quadVertexBufferPtr->texIndex = textureIndex;
        s_Data.quadVertexBufferPtr++;

        s_Data.quadVertexBufferPtr->position = { position.x + size.x, position.y + size.y, 0.0f };
        s_Data.quadVertexBufferPtr->color = color;
        s_Data.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
        s_Data.quadVertexBufferPtr->texIndex = textureIndex;
        s_Data.quadVertexBufferPtr++;

        s_Data.quadVertexBufferPtr->position = { position.x, position.y + size.y, 0.0f };
        s_Data.quadVertexBufferPtr->color = color;
        s_Data.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
        s_Data.quadVertexBufferPtr->texIndex = textureIndex;
        s_Data.quadVertexBufferPtr++;

        s_Data.quadIndexCount += 6;

        /*s_Data.quadShader->bind();
        s_Data.quadShader->setFloat4("u_Color", tintColor);
        s_Data.quadShader->setFloat("u_TilingFactor", tilingFactor);
        texture->bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data.quadShader->setMat4("u_Transform", transform);

        s_Data.quadVertexArray->bind();
        RendererCommand::drawIndexed(s_Data.quadVertexArray);*/
    }


    void const Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float angle, const glm::vec4& color)
    {
        drawRotatedQuad({ position.x, position.y, 0.0f }, size, angle, color);
    }
    void const Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float angle, const glm::vec4& color)
    {
        ARM_PROFILE_FUNCTION();

        s_Data.quadShader->bind();
        s_Data.quadShader->setFloat4("u_Color", color);
        s_Data.quadShader->setFloat("u_TilingFactor", 1.0f);
        s_Data.whiteTexture->bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), angle, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data.quadShader->setMat4("u_Transform", transform);

        s_Data.quadVertexArray->bind();
        RendererCommand::drawIndexed(s_Data.quadVertexArray);
    }
    void const Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float angle, const Ref<Texture2D> texture, const float tilingFactor, const glm::vec4& tintColor)
    {
        drawRotatedQuad({ position.x, position.y, 0.0f }, size, angle, texture, tilingFactor, tintColor);
    }
    void const Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float angle, const Ref<Texture2D> texture, const float tilingFactor, const glm::vec4& tintColor)
    {
        ARM_PROFILE_FUNCTION();

        s_Data.quadShader->bind();
        s_Data.quadShader->setFloat4("u_Color", tintColor);
        s_Data.quadShader->setFloat("u_TilingFactor", tilingFactor);
        texture->bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), angle, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data.quadShader->setMat4("u_Transform", transform);

        s_Data.quadVertexArray->bind();
        RendererCommand::drawIndexed(s_Data.quadVertexArray);
    }
}