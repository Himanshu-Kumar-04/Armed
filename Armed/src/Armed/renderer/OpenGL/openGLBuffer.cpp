#include "ArmPCH.h"
#include "openGLBuffer.h"
#include<glad.h>

namespace Arm {

    //////////////////////////////////////////
    //            VertexBuffer              //
    //////////////////////////////////////////

    OpenGLVertexBuffer::OpenGLVertexBuffer(const float* data, uint32_t size)
    {
        glCreateBuffers(1, &m_RendererID);
        bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }
    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }
    void OpenGLVertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER,m_RendererID);
    }
    void OpenGLVertexBuffer::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    //////////////////////////////////////////
    //            IndexBuffer               //
    //////////////////////////////////////////

    OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* data, uint32_t count)
        :m_Count(count)
    {
        glCreateBuffers(1, &m_RendererID);
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
    }
    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }
    void OpenGLIndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }
    void OpenGLIndexBuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
