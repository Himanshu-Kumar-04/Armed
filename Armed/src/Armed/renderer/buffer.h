#pragma once
#include "ArmPCH.h"
#include <vector>

namespace Arm {
    enum class ShaderDataType
    {
        None = 0, Float, Float2, Float3, Float4,
        Mat3, Mat4,
        Int, Int2, Int3, Int4,
        UInt, UInt2, UInt3, UInt4,
        Bool, UByte, Byte
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:    return 4;
        case ShaderDataType::Float2:   return 4 * 2;
        case ShaderDataType::Float3:   return 4 * 3;
        case ShaderDataType::Float4:   return 4 * 4;
        case ShaderDataType::Int:      return 4;
        case ShaderDataType::Int2:     return 4 * 2;
        case ShaderDataType::Int3:     return 4 * 3;
        case ShaderDataType::Int4:     return 4 * 4;
        case ShaderDataType::UInt:     return 4;
        case ShaderDataType::UInt2:    return 4 * 2;
        case ShaderDataType::UInt3:    return 4 * 3;
        case ShaderDataType::UInt4:    return 4 * 4;
        case ShaderDataType::Mat3:     return 4 * 3 * 3;
        case ShaderDataType::Mat4:     return 4 * 4 * 4;
        case ShaderDataType::Bool:     return 1;
        case ShaderDataType::UByte:    return 1;
        case ShaderDataType::Byte:     return 1;
        }

        ARM_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    struct BufferElement {
        std::string Name;
        ShaderDataType Type;
        uint32_t Size;
        uint32_t Offset;
        bool Normalized;

        BufferElement() = default;

        BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
            :Name(name), Type(type), Size(ShaderDataTypeSize(type)),Offset(0),Normalized(normalized)
        {
        }

        uint32_t GetComponentCount() const
        {
            switch (Type)
            {
            case ShaderDataType::Float:   return 1;
            case ShaderDataType::Float2:  return 2;
            case ShaderDataType::Float3:  return 3;
            case ShaderDataType::Float4:  return 4;
            case ShaderDataType::Mat3:    return 3; // 3* float3
            case ShaderDataType::Mat4:    return 4; // 4* float4
            case ShaderDataType::Int:     return 1;
            case ShaderDataType::Int2:    return 2;
            case ShaderDataType::Int3:    return 3;
            case ShaderDataType::Int4:    return 4;
            case ShaderDataType::UInt:    return 1;
            case ShaderDataType::UInt2:   return 2;
            case ShaderDataType::UInt3:   return 3;
            case ShaderDataType::UInt4:   return 4;
            case ShaderDataType::Bool:    return 1;
            case ShaderDataType::UByte:   return 1;
            case ShaderDataType::Byte:    return 1;
            }

            ARM_ASSERT(false, "Unknown ShaderDataType!");
            return 0;
        }
    };

    class BufferLayout {
    public:
        BufferLayout() = default;
        BufferLayout(std::initializer_list<BufferElement> elements)
            :m_Elements(elements)
        {
            calculateOffsetAndStride();
        }

        uint32_t getStride() const { return m_Stride; }
        const std::vector<BufferElement>& getElements() const { return m_Elements; }
        
        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
    private:
        void calculateOffsetAndStride() {
            uint32_t offset = 0;
            m_Stride = 0;
            for (auto& element : m_Elements) {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }
    private:
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
    };

    struct VertexData;

    class IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;
        virtual void bind() const {};
        virtual void unbind() const {};
        static Ref<IndexBuffer> Create(uint32_t* data, uint32_t count);
        virtual uint32_t GetCount() const = 0;
        virtual uint32_t getID() const = 0;
    };


    class VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;
        
        virtual void bind() const {};
        virtual void unbind() const {};

        virtual void setData(const void* data, uint32_t size) = 0;
        virtual void setIndexBuffer(Ref<IndexBuffer>& indexBuffer) = 0;
        virtual const Ref<IndexBuffer>& getIndexBuffer() const = 0;

        virtual const BufferLayout& getLayout() const = 0;
        virtual void setLayout(const BufferLayout& layout) = 0;

        static Ref<VertexBuffer> Create(std::vector<VertexData>& vertices);
        static Ref<VertexBuffer> Create(float* data, uint32_t size);
        static Ref<VertexBuffer> Create(uint32_t size);

        virtual uint32_t getID() const = 0;
    };

    
}