#pragma once
#include "rendererCommand.h"
#include "orthographicCamera.h"
#include "shader.h"
#include "texture.h"

namespace Arm {
    class Renderer2D {
    public:
        static void init();
        static void shutdown();

        static void beginScene(const OrthographicCamera& camera);
        static void endScene();

        static void const drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void const drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        static void const drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void const drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& tintColor = glm::vec4(1.0f));
    };
}