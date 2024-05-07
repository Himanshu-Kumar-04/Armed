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
        case Arm::ShaderDataType::Bool:         return GL_BOOL;
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
        glBindVertexArray(m_RendererID);
        vertexBuffer->bind();

        ARM_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertex Buffer has no layout");

        uint32_t index = 0;
        for (const auto& element : vertexBuffer->getLayout()) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                vertexBuffer->getLayout().getStride(),
                (const void*)element.Offset);
            index++;
        }

        m_VertexBuffer.push_back(vertexBuffer);
    }
    void OpenGLVertexArray::setIndexBuffer(Ref<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_RendererID);
        indexBuffer->bind();

        m_IndexBuffer = indexBuffer;
    }
}