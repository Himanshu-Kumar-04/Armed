#pragma once
#include "Armed/renderer/rendererAPI.h"

namespace Arm {
    class OpenGLRendererAPI : public RendererAPI {
    public:
        virtual void setclearColor(const glm::vec4& color);
        virtual void clearColor();

        virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray);
    };
}