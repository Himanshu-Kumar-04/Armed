#pragma once
#include"Armed/renderer/buffer.h"

namespace Arm {
    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(std::vector<VertexData> vertices);
        OpenGLVertexBuffer(uint32_t size);
        OpenGLVertexBuffer(const float* data, uint32_t size);
        ~OpenGLVertexBuffer();

        virtual void setData(const void* data, uint32_t size) override;
        virtual void setIndexBuffer(Ref<IndexBuffer>& indexBuffer) override;
        virtual const Ref<IndexBuffer>& getIndexBuffer() const override { return m_IndexBuffer; }


        virtual const BufferLayout& getLayout() const override { return m_Layout; }
        virtual void setLayout(const BufferLayout& layout) override;
        virtual uint32_t getID() const override { return m_VA_RendererID; }
    private:
        uint32_t m_VB_RendererID;
        uint32_t m_VA_RendererID;
        BufferLayout m_Layout;
        Ref<IndexBuffer> m_IndexBuffer;
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