#pragma once
#include "rendererCommand.h"
#include "camera.h"
#include "orthographicCamera.h"
#include "shader.h"


namespace Arm {
    enum class MeshType : uint8_t {
        STATIC = 0,
        DYNAMIC
    };
    enum class Shape : uint8_t {
        CUBE = 0,
    };

    class Renderer {
    public:
        static void init();
        static void onWindowResize(uint32_t width, uint32_t height);

        static void beginScene(Camera& camera);
        static void beginScene(OrthographicCamera& camera);
        static void endScene();
        static void submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const MeshType meshType = MeshType::DYNAMIC);
        static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

        //static void const DrawObject(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture2D, const glm::vec4& tintColor);
    private:
        struct SceneData {
            glm::mat4 viewMatrix;
            glm::mat4 projectionMatrix;
            glm::mat4 viewProjectionMatrix;

        };

        static Scope<SceneData> m_SceneData;
    };
}