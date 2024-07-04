#include "ArmPCH.h"
#include "openGLBuffer.h"
#include <glad.h>
#include "Armed/renderer/mesh.h"

namespace Arm {
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch (type)
        {
        case Arm::ShaderDataType::Float:        return GL_FLOAT;
        case Arm::ShaderDataType::Float2:       return GL_FLOAT;
        case Arm::ShaderDataType::Float3:       return GL_FLOAT;
        case Arm::ShaderDataType::Float4:       return GL_FLOAT;
        case Arm::ShaderDataType::Mat3:         return GL_FLOAT;
        case Arm::ShaderDataType::Mat4:         return GL_FLOAT;
        case Arm::ShaderDataType::Int:          return GL_INT;
        case Arm::ShaderDataType::Int2:         return GL_INT;
        case Arm::ShaderDataType::Int3:         return GL_INT;
        case Arm::ShaderDataType::Int4:         return GL_INT;
        case Arm::ShaderDataType::UInt:         return GL_UNSIGNED_INT;
        case Arm::ShaderDataType::UInt2:        return GL_UNSIGNED_INT;
        case Arm::ShaderDataType::UInt3:        return GL_UNSIGNED_INT;
        case Arm::ShaderDataType::UInt4:        return GL_UNSIGNED_INT;
        case Arm::ShaderDataType::Bool:         return GL_BOOL;
        case Arm::ShaderDataType::UByte:        return GL_UNSIGNED_BYTE;
        case Arm::ShaderDataType::Byte:         return GL_BYTE;
        }

        ARM_ASSERT(false, "Unknown ShaderDataType");
        return 0;
    }
    //////////////////////////////////////////
    //            VertexBuffer              //
    //////////////////////////////////////////

    OpenGLVertexBuffer::OpenGLVertexBuffer(std::vector<VertexData> vertices)
    {
        ARM_PROFILE_FUNCTION();

        glCreateVertexArrays(1, &m_VA_RendererID);

        glCreateBuffers(1, &m_VB_RendererID);
        glNamedBufferData(m_VB_RendererID, vertices.size() * sizeof(VertexData), vertices.data(), GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
    {
        ARM_PROFILE_FUNCTION();

        glCreateVertexArrays(1, &m_VA_RendererID);

        glCreateBuffers(1, &m_VB_RendererID);
        glNamedBufferData(m_VB_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(const float* data, uint32_t size)
    {
        ARM_PROFILE_FUNCTION();
        
        glCreateVertexArrays(1, &m_VA_RendererID);

        glCreateBuffers(1, &m_VB_RendererID);
        glNamedBufferData(m_VB_RendererID, size, data, GL_STATIC_DRAW);
    }
    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        ARM_PROFILE_FUNCTION();
        
        glDeleteVertexArrays(1, &m_VA_RendererID);
        glDeleteBuffers(1, &m_VB_RendererID);
    }

    void OpenGLVertexBuffer::setData(const void* data, uint32_t size)
    {
        glNamedBufferSubData(m_VB_RendererID, 0, size, data);
    }

    void OpenGLVertexBuffer::setIndexBuffer(Ref<IndexBuffer>& indexBuffer)
    {
        glVertexArrayElementBuffer(m_VA_RendererID, indexBuffer->getID());
        m_IndexBuffer = indexBuffer;
    }

    inline void OpenGLVertexBuffer::setLayout(const BufferLayout& layout) { 
        m_Layout = layout;
        
        ARM_ASSERT(m_Layout.getElements().size(), "Vertex Buffer has no layout");

        uint32_t index = 0;
        for (const auto& element : m_Layout) {
            glEnableVertexArrayAttrib(m_VA_RendererID, index);
            glVertexArrayAttribBinding(m_VA_RendererID, index, 0);
            glVertexArrayAttribFormat(m_VA_RendererID, index, element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE, element.Offset);
            glVertexArrayVertexBuffer(m_VA_RendererID, index, m_VB_RendererID, element.Offset, m_Layout.getStride());
            index++;
        }
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
