#pragma once
#include"Armed/renderer/vertexArray.h"

namespace Arm {
    class OpenGLVertexArray : public VertexArray {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual void addVertexBuffer(Ref<VertexBuffer>& vertexBuffer) override;
        virtual void setIndexBuffer(Ref<IndexBuffer>& indexBuffer) override;

        virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffer() const { return m_VertexBuffer; }
        virtual const Ref<IndexBuffer>& getIndexBuffer() const { return m_IndexBuffer; }
    private:
        uint32_t m_RendererID;
        std::vector<Ref<VertexBuffer>> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
    };
}