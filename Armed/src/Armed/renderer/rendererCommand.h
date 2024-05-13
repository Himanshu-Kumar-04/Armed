#pragma once
#include"rendererAPI.h"

namespace Arm {
    class RendererCommand {
    public:
        inline static void init() { s_RendererAPI->init(); }
        inline static void setClearColor(const glm::vec4& color) { s_RendererAPI->setclearColor(color); }
        inline static void clearColor() { s_RendererAPI->clearColor(); }
        inline static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
            s_RendererAPI->setViewport(x, y, width, height); 
        }

        inline static void drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) { s_RendererAPI->drawIndexed(vertexArray, indexCount); }
    private:
        static Scope<RendererAPI> s_RendererAPI;
    };
}