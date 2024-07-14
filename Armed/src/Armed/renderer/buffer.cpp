#include "ArmPCH.h"
#include "buffer.h"
#include "renderer.h"
#include "OpenGL/openGLBuffer.h"

namespace Arm {
    Ref<VertexBuffer> VertexBuffer::Create(std::vector<VertexData>& vertices)
    {
   

        switch (Renderer::getAPI()) {
        case RendererAPI::API::None:     std::cout << "RendererAPI: None is not supported.\n"; return nullptr;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLVertexBuffer>(vertices);
        }

        std::cout << "UNKNOWN RENDERER API\n";
        return nullptr;
    }
    Ref<VertexBuffer> VertexBuffer::Create(float* data, uint32_t size)
    {
   

        switch (Renderer::getAPI()) {
        case RendererAPI::API::None:     std::cout << "RendererAPI: None is not supported.\n"; return nullptr;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLVertexBuffer>(data,size);
        }

        std::cout << "UNKNOWN RENDERER API\n";
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
   

        switch (Renderer::getAPI()) {
        case RendererAPI::API::None:     std::cout << "RendererAPI: None is not supported.\n"; return nullptr;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLVertexBuffer>(size);
        }

        std::cout << "UNKNOWN RENDERER API\n";
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* data, uint32_t count)
    {
   

        switch (Renderer::getAPI()) {
        case RendererAPI::API::None:     std::cout << "RendererAPI: None is not supported.\n"; return nullptr;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLIndexBuffer>(data, count);
        }

        ARM_ASSERT(false, "Unknown Renderer API");
        return nullptr;
    }
}