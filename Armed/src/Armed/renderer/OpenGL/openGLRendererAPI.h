#pragma once
#include "Armed/renderer/rendererAPI.h"

namespace Arm {
    class OpenGLRendererAPI : public RendererAPI {
    public:
        virtual void init() override;
        virtual void setclearColor(const glm::vec4& color) override;
        virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        virtual void clearColor() override;
        virtual void enableDepthTest() override;
        virtual void disableDepthTest() override;
        virtual void drawIndexed(const Ref<VertexBuffer>& vertexBuffer, uint32_t indexCount = 0) override;
    };
}