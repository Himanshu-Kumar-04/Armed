#pragma once
#include"rendererAPI.h"

namespace Arm {
    class RendererCommand {
    public:
        inline static void init() { s_RendererAPI->init(); }
        inline static void setClearColor(const glm::vec4& color) { s_RendererAPI->setclearColor(color); }
        inline static void clearColor() { s_RendererAPI->clearColor(); }

        inline static void drawIndexed(const Ref<VertexArray>& vertexArray) { s_RendererAPI->drawIndexed(vertexArray); }
    private:
        static RendererAPI* s_RendererAPI;
    };
}