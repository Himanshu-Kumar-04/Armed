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
        glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(const float* data, uint32_t size)
    {
        ARM_PROFILE_FUNCTION();

        glCreateBuffers(1, &m_RendererID);
        glNamedBufferData(m_RendererID, size, data, GL_STATIC_DRAW);
    }
    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        ARM_PROFILE_FUNCTION();

        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLVertexBuffer::setData(const void* data, uint32_t size)
    {
        glNamedBufferSubData(m_RendererID, 0, size, data);
    }

    //////////////////////////////////////////
    //            IndexBuffer               //
    //////////////////////////////////////////

    OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* data, uint32_t count)
        :m_Count(count)
    {
        ARM_PROFILE_FUNCTION();

        glCreateBuffers(1, &m_RendererID);
        glNamedBufferData(m_RendererID, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
    }
    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        ARM_PROFILE_FUNCTION();

        glDeleteBuffers(1, &m_RendererID);
    }
}
