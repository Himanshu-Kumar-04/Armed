#include "ArmPCH.h"
#include "buffer.h"
#include "renderer.h"
#include "OpenGL/openGLBuffer.h"

namespace Arm {
    VertexBuffer* VertexBuffer::Create(float* data, uint32_t size)
    {
        switch (Renderer::getAPI()) {
        case RendererAPI::API::None:     std::cout << "RendererAPI: None is not supported.\n"; return nullptr;
        case RendererAPI::API::OpenGL:   return new OpenGLVertexBuffer(data,size);
        }

        std::cout << "UNKNOWN RENDERER API\n";
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* data, uint32_t count)
    {
        switch (Renderer::getAPI()) {
        case RendererAPI::API::None:     std::cout << "RendererAPI: None is not supported.\n"; return nullptr;
        case RendererAPI::API::OpenGL:   return new OpenGLIndexBuffer(data, count);
        }

        ASSERT(false, "Unknown Renderer API");
        return nullptr;
    }
}