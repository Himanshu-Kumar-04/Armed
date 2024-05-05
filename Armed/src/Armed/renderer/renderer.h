#pragma once
#include "rendererCommand.h"
#include "orthographicCamera.h"
#include "shader.h"

namespace Arm {
    class Renderer {
    public:
        static void beginScene(OrthographicCamera& camera);
        static void endScene();

        static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

        static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
    private:
        struct SceneData {
            glm::mat4 viewProjectionMatrix;
        };

        static SceneData* m_SceneData;
    };
}