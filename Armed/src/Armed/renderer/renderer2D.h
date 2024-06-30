#pragma once
#include "rendererCommand.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

namespace Arm {
    class Renderer2D {
    public:
        static void init();
        static void shutdown();

        static void beginBatch();
        static void flush();

        static void beginScene(const Camera& camera, const glm::mat4& transform);
        static void beginScene(const glm::mat4& transform);
        static void endScene();

        static void const drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void const drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        static void const drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, const float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void const drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

        static void const drawRotatedQuad(const glm::vec2& position, const glm::vec2& size , const float angle, const glm::vec4& color);
        static void const drawRotatedQuad(const glm::vec3& position, const glm::vec2& size , const float angle, const glm::vec4& color);
        static void const drawRotatedQuad(const glm::vec2& position, const glm::vec2& size , const float angle, const Ref<Texture2D> texture, const float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void const drawRotatedQuad(const glm::vec3& position, const glm::vec2& size , const float angle, const Ref<Texture2D> texture, const float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

        static void const drawQuad(const glm::mat4& transform, const glm::vec4& color);
        static void const drawQuad(const glm::mat4& transform, const Ref<Texture2D> texture, const float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        //Stats
        struct Statistics {
            uint32_t drawCalls;
            uint32_t quadCount;
            uint32_t getTotalVertexCount() { return quadCount * 4; }
            uint32_t getTotalIndexCount() { return quadCount * 6; }
        };

        static Statistics getStats();
        static void resetStats();
    private:
        static void nextBatch();
    };
}