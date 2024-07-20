#include "ArmPCH.h"
#include <Armed/renderer/RenderCommand.h>
#include "renderer2D.h"
#include<glm/gtc/matrix_transform.hpp>



namespace Arm {
    struct QuadVertex {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoord;
        float texIndex;
        float tilingFactor;
    };

    struct Renderer2DData {
        static const uint32_t maxQuadCount = 20000;
        static const uint32_t maxVertexCount = maxQuadCount * 4;
        static const uint32_t maxIndexCount = maxQuadCount * 6;
        static const uint32_t maxTextureSlots = 32; // TODO: RENDER_CAPS

        Ref<VertexBuffer> quadVertexBuffer;
        Ref<Shader> quadShader;
        Ref<Texture2D> whiteTexture;

        uint32_t quadIndexCount = 0;
        QuadVertex* quadVertexBufferBase = nullptr;
        QuadVertex* quadVertexBufferPtr = nullptr;

        std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
        uint32_t textureSlotIndex = 1;

        glm::vec4 quadVertexPositions[4];

        Renderer2D::Statistics stats;
    };

    static Renderer2DData s_R2DData;

    void Renderer2D::init()
    {
   

        s_R2DData.quadVertexBuffer = VertexBuffer::Create(s_R2DData.maxVertexCount * sizeof(QuadVertex));
        s_R2DData.quadVertexBuffer->setLayout({
                {ShaderDataType::Float3 , "position"},
                {ShaderDataType::Float4 , "color"},
                {ShaderDataType::Float2 , "texCoord"},
                {ShaderDataType::Float , "texIndex"},
                {ShaderDataType::Float , "tilingFactor"}
            });
        s_R2DData.quadVertexBufferBase = new QuadVertex[s_R2DData.maxVertexCount];

        uint32_t* quadIndices = new uint32_t[s_R2DData.maxIndexCount];
        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_R2DData.maxIndexCount; i += 6) {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;
            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;
            offset += 4;
        }
        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_R2DData.maxIndexCount);
        s_R2DData.quadVertexBuffer->setIndexBuffer(quadIB);
        delete[] quadIndices;

        s_R2DData.whiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_R2DData.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

        int32_t sampler[s_R2DData.maxTextureSlots];
        for (uint32_t i = 0; i < s_R2DData.maxTextureSlots; i++) {
            sampler[i] = i;
        }
        s_R2DData.quadShader = Shader::create("assets/shader/texture2D.glsl");
        s_R2DData.quadShader->bind();
        s_R2DData.quadShader->setIntArray("u_Textures",sampler, s_R2DData.maxTextureSlots);

        s_R2DData.textureSlots[0] = s_R2DData.whiteTexture;

        s_R2DData.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_R2DData.quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        s_R2DData.quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        s_R2DData.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
    }
    
    void Renderer2D::shutdown()
    {
   
        delete[] s_R2DData.quadVertexBufferBase;
    }

    //////////////////////////////////////
    // Batches
    ////////////////////////////////////// 

    void Renderer2D::beginBatch() {
        s_R2DData.quadIndexCount = 0;
        s_R2DData.quadVertexBufferPtr = s_R2DData.quadVertexBufferBase;
        s_R2DData.textureSlotIndex = 1;
    }

    void Renderer2D::flush() {
        uint32_t dataSize = (uint32_t)((uint8_t*)s_R2DData.quadVertexBufferPtr - (uint8_t*)s_R2DData.quadVertexBufferBase);
        s_R2DData.quadVertexBuffer->setData(s_R2DData.quadVertexBufferBase, dataSize);
        for (uint32_t i = 0; i < s_R2DData.textureSlotIndex; i++) {
            s_R2DData.textureSlots[i]->bind(i);
        }
        RenderCommand::drawIndexed(s_R2DData.quadVertexBuffer, s_R2DData.quadIndexCount);
        s_R2DData.stats.drawCalls++;
    }

    void Renderer2D::nextBatch() {
        endScene();
        beginBatch();
    }

    //////////////////////////////////////
    // Scenes
    ////////////////////////////////////// 

    void Renderer2D::beginScene(const Camera& camera, const glm::mat4& transform)
    {
        s_R2DData.quadShader->setMat4("u_ViewProjection", camera.getProjection() * glm::inverse(transform));
        beginBatch();
    }

    void Renderer2D::beginScene(const glm::mat4& transform)
    {
        s_R2DData.quadShader->setMat4("u_ViewProjection", transform);
        beginBatch();
    }

    void Renderer2D::endScene()
    {
   

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
   

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        drawQuad(transform, color);
    }
    void const Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, const float tilingFactor, const glm::vec4& tintColor)
    {
        drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
    }
    void const Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const float tilingFactor, const glm::vec4& tintColor)
    {
   
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        drawQuad(transform, texture, tilingFactor, tintColor);
    }


    void const Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float angle, const glm::vec4& color)
    {
        drawRotatedQuad({ position.x, position.y, 0.0f }, size, angle, color);
    }
    void const Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float angle, const glm::vec4& color)
    {
   

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), angle, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        drawQuad(transform, color);
    }
    void const Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float angle, const Ref<Texture2D> texture, const float tilingFactor, const glm::vec4& tintColor)
    {
        drawRotatedQuad({ position.x, position.y, 0.0f }, size, angle, texture, tilingFactor, tintColor);
    }
    void const Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float angle, const Ref<Texture2D> texture, const float tilingFactor, const glm::vec4& tintColor)
    {
   

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), angle, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        drawQuad(transform, texture, tilingFactor, tintColor);
    }


    void const Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color)
    {
   

        if (s_R2DData.quadIndexCount >= Renderer2DData::maxIndexCount)
            nextBatch();

        constexpr glm::vec2 texCoords[] = {{0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f}};

        const float textureIndex = 0.0f;
        const float tilingFactor = 1.0f;

        for (uint8_t i = 0; i < 4; i++) {
            s_R2DData.quadVertexBufferPtr->position = transform * s_R2DData.quadVertexPositions[i];
            s_R2DData.quadVertexBufferPtr->color = color;
            s_R2DData.quadVertexBufferPtr->texCoord = texCoords[i];
            s_R2DData.quadVertexBufferPtr->texIndex = textureIndex;
            s_R2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
            s_R2DData.quadVertexBufferPtr++;
        }
        s_R2DData.quadIndexCount += 6;
        s_R2DData.stats.quadCount++;
    }
    void const Renderer2D::drawQuad(const glm::mat4& transform, const Ref<Texture2D> texture, const float tilingFactor, const glm::vec4& tintColor)
    {
   
        
        if (s_R2DData.quadIndexCount >= Renderer2DData::maxIndexCount)
            nextBatch();

        constexpr glm::vec2 texCoords[] = { {0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f} };

        float textureIndex = 0.0f;

        for (uint32_t i = 1; i < s_R2DData.textureSlotIndex; i++) {
            if (*s_R2DData.textureSlots[i].get() == *texture.get()) {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0) {
            textureIndex = (float)s_R2DData.textureSlotIndex;
            s_R2DData.textureSlots[s_R2DData.textureSlotIndex] = texture;
            s_R2DData.textureSlotIndex++;
        }

        for (uint8_t i = 0; i < 4; i++) {
            s_R2DData.quadVertexBufferPtr->position = transform * s_R2DData.quadVertexPositions[i];
            s_R2DData.quadVertexBufferPtr->color = tintColor;
            s_R2DData.quadVertexBufferPtr->texCoord = texCoords[i];
            s_R2DData.quadVertexBufferPtr->texIndex = textureIndex;
            s_R2DData.quadVertexBufferPtr->tilingFactor = tilingFactor;
            s_R2DData.quadVertexBufferPtr++;
        }
        s_R2DData.quadIndexCount += 6;
        s_R2DData.stats.quadCount++;
    }
    Renderer2D::Statistics Renderer2D::getStats()
    {
        return s_R2DData.stats;
    }
    void Renderer2D::resetStats()
    {
        memset(&s_R2DData.stats, 0, sizeof(Statistics));
    }

}