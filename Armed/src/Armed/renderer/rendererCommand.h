#pragma once
#include"rendererAPI.h"

namespace Arm {
    class RendererCommand {
    public:
        inline static void setClearColor(const glm::vec4& color) { m_RendererAPI->setclearColor(color); }
        inline static void clearColor() { m_RendererAPI->clearColor(); }

        inline static void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) { m_RendererAPI->drawIndexed(vertexArray); }
    private:
        static RendererAPI* m_RendererAPI;
    };
}