#pragma once
#include"Armed/renderer/vertexArray.h"

namespace Arm {
    class OpenGLVertexArray : public VertexArray {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual void addVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer) override;
        virtual void setIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) override;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffer() const { return m_VertexBuffer; }
        virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() const { return m_IndexBuffer; }
    private:
        uint32_t m_RendererID;
        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
    };
}