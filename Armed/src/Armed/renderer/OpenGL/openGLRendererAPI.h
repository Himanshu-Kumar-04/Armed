#pragma once
#include "Armed/renderer/rendererAPI.h"

namespace Arm {
    class OpenGLRendererAPI : public RendererAPI {
    public:
        virtual void init() override;
        virtual void setclearColor(const glm::vec4& color) override;
        virtual void clearColor() override;

        virtual void drawIndexed(const Ref<VertexArray>& vertexArray) override;
    };
}