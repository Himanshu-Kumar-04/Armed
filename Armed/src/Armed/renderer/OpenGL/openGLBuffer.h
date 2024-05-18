#pragma once
#include"Armed/renderer/buffer.h"

namespace Arm {
    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(uint32_t size);
        OpenGLVertexBuffer(const float* data, uint32_t size);
        ~OpenGLVertexBuffer();

        virtual void setData(const void* data, uint32_t size) override;

        virtual const BufferLayout& getLayout() const override { return m_Layout; }
        virtual void setLayout(const BufferLayout& layout) override { m_Layout = layout; }
        virtual uint32_t getID() const override { return m_RendererID; }
    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
    };

    class OpenGLIndexBuffer : public IndexBuffer {
    public:
        OpenGLIndexBuffer(const uint32_t* data, uint32_t count);
        ~OpenGLIndexBuffer();
        virtual uint32_t GetCount() const override { return m_Count; }
        virtual uint32_t getID() const override { return m_RendererID; }
    private:
        uint32_t m_RendererID,m_Count;
    };
}