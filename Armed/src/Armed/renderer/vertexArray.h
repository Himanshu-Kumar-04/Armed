#pragma once
#include"buffer.h"

namespace Arm {
    class VertexArray {
    public:
        ~VertexArray() = default;
        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void addVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer) =  0;
        virtual void setIndexBuffer(std::shared_ptr<IndexBuffer>& vertexBuffer) = 0;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffer() const = 0;
        virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() const = 0;

        static VertexArray* Create();
    };
}