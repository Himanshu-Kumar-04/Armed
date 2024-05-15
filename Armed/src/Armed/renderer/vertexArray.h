#pragma once
#include"buffer.h"

namespace Arm {
    class VertexArray {
    public:
        ~VertexArray() = default;
        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void addVertexBuffer(Ref<VertexBuffer>& vertexBuffer) =  0;
        virtual void setIndexBuffer(Ref<IndexBuffer>& indexBuffer) = 0;

        virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffer() const = 0;
        virtual const Ref<IndexBuffer>& getIndexBuffer() const = 0;

        static Ref<VertexArray> Create();
    };
}