#pragma once
#include "RenderCommand.h"
#include "camera.h"
#include "shader.h"
#include "mesh.h"


namespace Arm {
    class Renderer {
    public:
        static void init();
        static void onWindowResize(uint32_t width, uint32_t height);

        static void beginScene(const Camera& camera, const glm::mat4& transform);
        static void endScene();
        static void submit(const glm::mat4& transform, Mesh& mesh);
        static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
        static void setAPI(RendererAPI::API api) { RendererAPI::setAPI(api); }

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