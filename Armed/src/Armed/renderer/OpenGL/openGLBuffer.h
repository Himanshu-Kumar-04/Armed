#pragma once
#include"Armed/renderer/buffer.h"

namespace Arm {
    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(const float* data, uint32_t size);
        ~OpenGLVertexBuffer();
        virtual void bind() const override;
        virtual void unbind() const override;

        virtual const BufferLayout& getLayout() const override { return m_Layout; }
        virtual void setLayout(const BufferLayout& layout) override { m_Layout = layout; }
    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
    };

    class OpenGLIndexBuffer : public IndexBuffer {
    public:
        OpenGLIndexBuffer(const uint32_t* data, uint32_t count);
        ~OpenGLIndexBuffer();
        virtual void bind() const override;
        virtual void unbind() const override;
        virtual uint32_t GetCount() const { return m_Count; }

    private:
        uint32_t m_RendererID,m_Count;
    };
}