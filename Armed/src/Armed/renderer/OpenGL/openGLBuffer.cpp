#include "ArmPCH.h"
#include "openGLBuffer.h"
#include<glad.h>

namespace Arm {

    //////////////////////////////////////////
    //            VertexBuffer              //
    //////////////////////////////////////////

    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
    {
        ARM_PROFILE_FUNCTION();

        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(const float* data, uint32_t size)
    {
        ARM_PROFILE_FUNCTION();

        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }
    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        ARM_PROFILE_FUNCTION();

        glDeleteBuffers(1, &m_RendererID);
    }
    void OpenGLVertexBuffer::bind() const
    {
        ARM_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER,m_RendererID);
    }
    void OpenGLVertexBuffer::unbind() const
    {
        ARM_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::setData(const void* data, uint32_t size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    //////////////////////////////////////////
    //            IndexBuffer               //
    //////////////////////////////////////////

    OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* data, uint32_t count)
        :m_Count(count)
    {
        ARM_PROFILE_FUNCTION();

        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
    }
    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        ARM_PROFILE_FUNCTION();

        glDeleteBuffers(1, &m_RendererID);
    }
    void OpenGLIndexBuffer::bind() const
    {
        ARM_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }
    void OpenGLIndexBuffer::unbind() const
    {
        ARM_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
