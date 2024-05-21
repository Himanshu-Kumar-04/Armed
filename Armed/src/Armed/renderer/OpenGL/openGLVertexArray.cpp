#include "ArmPCH.h"
#include "openGLVertexArray.h"
#include <glad.h>

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

    OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1, &m_RendererID);
    }
    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID);
    }
    void OpenGLVertexArray::bind() const
    {
        glBindVertexArray(m_RendererID);
    }
    void OpenGLVertexArray::unbind() const
    {
        glBindVertexArray(0);
    }
    void OpenGLVertexArray::addVertexBuffer(Ref<VertexBuffer>& vertexBuffer)
    {
        ARM_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertex Buffer has no layout");

        uint32_t index = 0;
        for (const auto& element : vertexBuffer->getLayout()) {
            glEnableVertexArrayAttrib(m_RendererID, index);
            glVertexArrayAttribBinding(m_RendererID,index, 0);
            glVertexArrayAttribFormat(m_RendererID,index,element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE, element.Offset);
            glVertexArrayVertexBuffer(m_RendererID,index,vertexBuffer->getID(),element.Offset, vertexBuffer->getLayout().getStride());
            index++;
        }

        m_VertexBuffer.push_back(vertexBuffer);
    }
    void OpenGLVertexArray::setIndexBuffer(Ref<IndexBuffer>& indexBuffer)
    {
        glVertexArrayElementBuffer(m_RendererID,indexBuffer->getID());
        m_IndexBuffer = indexBuffer;
    }
}